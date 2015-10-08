/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: sxdkl
**  File:    sxdkl_uart.c
**  Author:  wangdawei
**  Date:    08/10/2015
**
**  Purpose:
**    千帕串口通用协议.
**************************************************************************/


/* Include files. */
#include "includes.h"
#include "smart_config.h"
#include "ds_proto.h"
#include "uc_dev.h"
#include "sxdkl_uart.h"
#include "uc_dev_event.h"
#include "uc_push.h"
#include "user_config.h"
#include "mu_type.h"
#include "uc_dev_timer.h"

/* Macro constant definitions. */

#define SXDKL_DEBUG

#ifdef SXDKL_DEBUG
#define SXDKL_INFO printf
//#define SXDKL_INFO(fmt...) DBGPRINT_MGMT(DEBUG_INFO, fmt)
#else
#define SXDKL_INFO INFO
#endif

#define FUNC_INFO SXDKL_INFO("enter function:%s[file:%s,line:%d]\n", __FUNCTION__, __FILE__, __LINE__);

/* Type definitions. */
enum {
	//就绪状态，可以发送命令
	SXDKL_UART_STATE_READY = 0x0,
	//就绪状态，等待回包或超时后在发送
	SXDKL_UART_STATE_WORKING = 0x1
};

#define  SXDKL_MAX_UART_BD_NUM              3
#define  SXDKL_UART_BUFF_MAX_SIZE          32
#define  SXDKL_UART_SEND_TIMEOUT 			1
#define  SXDKL_UART_SEND_LIST_MAX			25
#define  SXDKL_UART_SEND_TRY_MAX			3
#define  SXDKL_UART_CMD_SYNC				0xAA
#ifdef SXDKL_DEBUG
#define SXDKL_QUERY_TIME			100000
#else
#define SXDKL_QUERY_TIME			500
#endif

#define SXDKL_UART_CMD_SET				3
#define SXDKL_UART_CMD_GET				2
#define SXDKL_UART_CMD_WIFI_STAT		1


typedef struct {
	u_int8_t has_data;
	u_int8_t data_len;
	u_int8_t data[SXDKL_UART_BUFF_MAX_SIZE];
} sxdkl_uart_db_t;


typedef enum {
	SXDKL_NONE,
	SXDKL_DATA
}sxdkl_uart_state_t;



typedef struct {
	struct stlc_list_head link;	
	u_int8_t try_conut;
	u_int8_t data_len;
} sxdkl_uart_pkt_t;

typedef struct {
	bool init_done;
	//信号量，串口发送时锁住
	OS_EVENT *mutext_out;
	//串口接收数据的缓存
	sxdkl_uart_db_t *cur_uart_db;
	sxdkl_uart_db_t uart_db[SXDKL_MAX_UART_BD_NUM];
       u_int8_t bd_parse_begin;
	//串口读取状态机
	u_int8_t state;
	//数据长度
	u_int8_t param_len;
	u_int16_t index_read;
	u_int32_t last_input_time;		// 最后一次串口输入的时间
}sxdkl_uart_t;

typedef struct{
	u_int8_t router_stat:1;		//Bit0：路由连接状态：0：disconnect   1：connect;
	u_int8_t net_stat:1;		//Bit1：网络连接状态：0：disconnect   1：connect;
	u_int8_t reset_suc:1;    	//Bit2：复位成功状态： 0：失败，1：成功； 
	u_int8_t wifi_result:1; 	//Bit3：wifi配网结果： 0：未直连；1：成功；  
	u_int8_t wifi_test_res:1;	//Bit4: wifi厂测结果： 0：未直连；1：成功
	u_int8_t wifi_disbind_res:1;//Bit5：wifi解除绑定结果： 0：未直连；1：成功
	u_int8_t pad:2;
}wifi_bits;


typedef struct {
	u_int8_t sync;
	u_int8_t cmd;
	u_int8_t func_bits;
	u_int8_t level_bits;
	u_int8_t speed_bits;
	u_int8_t temp;//设定温度：15-32C(90-60F)
	u_int8_t timer_bits;
	u_int8_t mins;//定时分钟：00-59分
	wifi_bits wb;//wifi状态位
	u_int8_t wifi_signal;//WIFI信号强度0-99
	u_int8_t pad;
	u_int8_t checksum;
}sxdkl_cmd_t;

/* Local function declarations. */
extern void serial_active(u_int8_t ch);
void sxdkl_uart_do_cmd(sxdkl_uart_db_t * bd);
void sxdkl_uart_send(u_int8_t * data, u_int8_t len);
void sxdkl_uart_input(void);
void sxdkl_uart_request_send(void);
int sxdkl_uart_wifi_stat(wifi_bits *wb);
/* Macro API definitions. */


/* Global variable declarations. */
static cl_thread_t *die_timer = NULL;
static struct stlc_list_head sxdkl_uart_send_list;
int sxdkl_send_list_state = SXDKL_UART_STATE_READY;
static sxdkl_uart_t sxdkl_uart;

sxdkl_res_t sxdkl_stat;
u_int8_t wifi_mode = WIFI_MODE_STA;		//0---ap  1---sta
static int sxdkl_uart_die(cl_thread_t *t)
{
	die_timer = NULL;

	sxdkl_uart_request_send();

	return 0;
}

static void sxdkl_uart_die_timer_on()
{
	if (die_timer){
		CL_THREAD_OFF(die_timer);
	}
	CL_THREAD_TIMER_ON(MASTER, die_timer, sxdkl_uart_die, NULL, TIME_N_SECOND(SXDKL_UART_SEND_TIMEOUT));
}

static void sxdkl_uart_die_timer_off()
{
	if (die_timer){
		CL_THREAD_OFF(die_timer);
	}
}

static void init_sxdkl_uart_send_list()
{
	STLC_INIT_LIST_HEAD(&sxdkl_uart_send_list);
}

static void sxdkl_uart_request_del()
{
	sxdkl_uart_pkt_t *pkt;

	if (stlc_list_empty(&sxdkl_uart_send_list))
		return;
	
	pkt = (sxdkl_uart_pkt_t *)stlc_list_entry(sxdkl_uart_send_list.next, sxdkl_uart_pkt_t, link);
	stlc_list_del(&pkt->link);
	pkt_free(pkt);
}
static u_int8_t sxdkl_get_checksum(u_int8_t *data, u_int8_t len) 
{
	int sum = 0, i = 0;
	if (!data) {
		return 0;
	}
	for (i = 0; i < len; i++){
		sum += data[i];
	}
	return sum & 0xff;
}
static sxdkl_uart_pkt_t *sxdkl_uart_pkt_generate(u_int8_t cmd, u_int8_t *data, 
																u_int8_t len)
{
	u_int8_t total_len, i;
	int sum = 0;
	sxdkl_uart_pkt_t *pkt = NULL;
	sxdkl_cmd_t *sct;
	u_int8_t *ptr;
   	if (true != sxdkl_uart.init_done){
		SXDKL_INFO("sxdkl uart not init!\n");
		return NULL;
	}
    if (NULL == data && len != 0){
		SXDKL_INFO("sxdkl uart send param error!\n");
		return NULL;
    }
	total_len = sizeof(sxdkl_uart_pkt_t) + sizeof(sxdkl_cmd_t);
	pkt = (sxdkl_uart_pkt_t *)cl_calloc(total_len, 1);
	if (!pkt){
		SXDKL_INFO("cl_calloc error!\n");
		return NULL;
	}
	sct = &pkt[1];
	sct->sync = SXDKL_UART_CMD_SYNC;
	sct->cmd = cmd;
	switch(cmd){
		case SXDKL_UART_CMD_GET:
			break;
		case SXDKL_UART_CMD_WIFI_STAT:
			memcpy(&sct->wb, data, sizeof(u_int8_t));
			break;
		case SXDKL_UART_CMD_SET:
			memcpy(sct, data, len);
			break;
		default:
			SXDKL_INFO("unknow cmd, canceled!\n");
			cl_free(pkt);
			return NULL;
	}
	ptr = (u_int8_t *)(pkt + 1);
    sct->checksum = sxdkl_get_checksum(&ptr[1], sizeof(sxdkl_cmd_t) - 2);
	SXDKL_INFO("send crc is %u!\n", sct->checksum);
	
	pkt->data_len = sizeof(sxdkl_cmd_t);
	pkt->try_conut = 0; 
	return pkt;
}
static int sxdkl_uart_request_add(u_int8_t cmd, u_int8_t *data, u_int8_t data_len)
{
	sxdkl_uart_pkt_t *pkt = NULL;
	int count = 0;
	stlc_list_count(count, &sxdkl_uart_send_list);

	if(count >= SXDKL_UART_SEND_LIST_MAX) {
		SXDKL_INFO( "sxdkl uart send list is full, replace the last one\n");
		if(sxdkl_uart_send_list.prev) {
			pkt = (sxdkl_uart_pkt_t *)stlc_list_entry(sxdkl_uart_send_list.prev, sxdkl_uart_pkt_t, link);
			stlc_list_del(&pkt->link);
			pkt_free(pkt);
		} else {
			return -1;
		}
	}
	pkt = sxdkl_uart_pkt_generate(cmd, data, data_len);
	if (!pkt){
		SXDKL_INFO("sxdkl_uart_pkt_generate failed!\n");
		return -1;
	}
	stlc_list_add_tail(&pkt->link, &sxdkl_uart_send_list);
	if(sxdkl_send_list_state == SXDKL_UART_STATE_READY) {
		//串口就绪，马上发送
		sxdkl_uart_request_send();
	}
	return 0;
}
static void sxdkl_uart_request_send()
{
	sxdkl_uart_pkt_t *pkt;

	if (stlc_list_empty(&sxdkl_uart_send_list))
		return;
	
	pkt = (sxdkl_uart_pkt_t *)stlc_list_entry(sxdkl_uart_send_list.next, sxdkl_uart_pkt_t, link);
	if(pkt) {
		sxdkl_send_list_state = SXDKL_UART_STATE_WORKING;
		pkt->try_conut++;

		if(pkt->try_conut > SXDKL_UART_SEND_TRY_MAX) {
			//超过最大重发次数丢弃发送下一个
			sxdkl_send_list_state = SXDKL_UART_STATE_READY;
			sxdkl_uart_request_del();
			sxdkl_uart_request_send();
		} else {
			SXDKL_INFO("try_count=%u\n", pkt->try_conut);
			sxdkl_uart_send(pkt + 1, pkt->data_len);
		}
		//启用超时定时器
		sxdkl_uart_die_timer_on();
	}
}
static void sxdkl_uart_send(u_int8_t *data, u_int8_t data_len)
{
	u_int16_t i = 0;
	u_int8_t err = 0;
	if (data == NULL) {
		return;
	}
	OSMutexPend(sxdkl_uart.mutext_out, 0, &err);

	// 写数据
	for (i = 0; i < data_len; i++) {
		BSP_UartPutcPolled(data[i]);
	}
	OSMutexPost(sxdkl_uart.mutext_out);
}


static void sxdkl_uart_input(void)
{
	u_int8_t value = 0;
    int i = 0;
	// 长时间无数据输入，而之前有收到部分报文，做丢弃处理
	if (get_msec() - sxdkl_uart.last_input_time > 300) {
		if (sxdkl_uart.state != SXDKL_NONE) {
			SXDKL_INFO("drop prev uart:stat=%u, idx_read=%u\n", 
				sxdkl_uart.state, sxdkl_uart.index_read);
			sxdkl_uart.index_read = 0;
			sxdkl_uart.state = SXDKL_NONE;
		}
	}

	if ((NST_RD_UART_REG(LSR_OFFSET) & LSR_DR_MASK) == 0) {
		SXDKL_INFO("LSR_OFFSET ERROR\r\n");
		return;
	}
	
	value = NST_RD_UART_REG(RBR_OFFSET);
	//如果收到的是+++，则进行导入证书流程
	serial_active(value);
	ucd_event_call(EVENT_UART_VALUE, &value, sizeof(value));

	//存放数据缓存满了
	if (sxdkl_uart.cur_uart_db->has_data) {
		SXDKL_INFO("uart bd full\n");
		goto err;
	}

	sxdkl_uart.last_input_time = get_msec();
	switch (sxdkl_uart.state) {
        	case SXDKL_NONE:
        		if (SXDKL_UART_CMD_SYNC == value){
                    sxdkl_uart.cur_uart_db->data[sxdkl_uart.index_read++] = value;
                    sxdkl_uart.state = SXDKL_DATA;
    			} 
        	break;
        	case SXDKL_DATA:
				if (sxdkl_uart.index_read >= SXDKL_UART_BUFF_MAX_SIZE){
					goto err;
				}
				sxdkl_uart.cur_uart_db->data[sxdkl_uart.index_read++] = value;
				if (sizeof(sxdkl_res_t) == sxdkl_uart.index_read){
					goto done;
				}
				break;
	}
	return;
done:
	//数据校验
	if (sxdkl_get_checksum(&sxdkl_uart.cur_uart_db->data[1], 
		sizeof(sxdkl_res_t) - 2) != value) {
		SXDKL_INFO("data crc check  err\n");				
		goto err;
	}
	sxdkl_uart.cur_uart_db->has_data = true;
	value = sxdkl_uart.cur_uart_db - &sxdkl_uart.uart_db[0];	
	sxdkl_uart.cur_uart_db = &sxdkl_uart.uart_db[(value + 1)%SXDKL_MAX_UART_BD_NUM];

err:
	sxdkl_uart.index_read  = 0;
	sxdkl_uart.param_len = 0;
	sxdkl_uart.state = SXDKL_NONE;
}

static void sxdkl_uart_callback(int event, void *data, int data_len)
{
	sxdkl_uart_input();
}
static void sxdkl_config_save()
{
	conf_dev_set(DEV_SXDKL_CONFIG, &sxdkl_stat, sizeof(sxdkl_res_t));
	conf_dev_set(DEV_SXDKL_BOOT_CONFIG, &wifi_mode, sizeof(u_int8_t));
}
void sxdkl_config_init()
{
	config_t *config;
	
	config = conf_dev_get(DEV_SXDKL_CONFIG);
	if (!config || !config->value || !config->len){
		return;
	}
	memcpy(&sxdkl_stat, config->value, sizeof(sxdkl_res_t));

	config = conf_dev_get(DEV_SXDKL_BOOT_CONFIG);
	if (!config || !config->value || !config->len){
		return;
	}
	memcpy(&wifi_mode, config->value, sizeof(u_int8_t));
	SXDKL_INFO("wifi mode:%u\n", wifi_mode);
}
static void wifi_mode_switch(u_int8_t mode)
{
	wifi_mode = mode;
	sxdkl_config_save();
	Reboot_Handler();
}

static void uart_ctl_handle(wifi_ctl_bits *wcb)
{
	wifi_bits wb;
	memset(&wb, 0, sizeof(wifi_bits));
	if (wcb->reset){
		wb.reset_suc = 1;
		sxdkl_uart_wifi_stat(&wb);
		config_reset();
		return;
	}
	if (wcb->wifi_disbind){
		wb.wifi_disbind_res = 1;
		sxdkl_uart_wifi_stat(&wb);
		wifi_mode_switch(WIFI_MODE_AP);
		return;
	}
	if (wcb->wifi_test){
		wb.wifi_test_res = 1;
		sxdkl_uart_wifi_stat(&wb);
		return;
	}
}
static void sxdkl_uart_do_cmd(sxdkl_uart_db_t *bd)
{
	sxdkl_res_t *srt = (sxdkl_res_t*)bd->data;
	if (memcmp(&sxdkl_stat, srt, sizeof(sxdkl_res_t))){
		memcpy(&sxdkl_stat, srt, sizeof(sxdkl_res_t));
		sxdkl_config_save();
		uart_ctl_handle(&srt->work_stat);
		uc_push_set();
	}
}
static void sxdkl_uart_cmd_parse(int event, void *data, int data_len)
{
	int i;
	sxdkl_uart_db_t *bd = NULL;
	sxdkl_res_t *srt;
	for (i = 0; i < SXDKL_MAX_UART_BD_NUM; i++) {
		bd = &sxdkl_uart.uart_db[sxdkl_uart.bd_parse_begin];
		if ( !bd->has_data ){
			break;
		}
		
		SXDKL_INFO("parser do cmd\n");
		
		srt = (sxdkl_res_t*)bd->data;
		if (!srt->cmd_ack.recv_suc){
			SXDKL_INFO("data error, resend...\n");
			break;
		}
		sxdkl_uart_do_cmd(bd);

		sxdkl_send_list_state = SXDKL_UART_STATE_READY;
		sxdkl_uart_die_timer_off();
		sxdkl_uart_request_del();
		sxdkl_uart_request_send();
		bd->has_data = false;
		sxdkl_uart.bd_parse_begin = (sxdkl_uart.bd_parse_begin + 1)%SXDKL_MAX_UART_BD_NUM;
	}
}
void sxdkl_ap_to_sta()
{
	wifi_bits wb;
	wb.wifi_disbind_res = 1;
	sxdkl_uart_wifi_stat(&wb);
	wifi_mode_switch(WIFI_MODE_STA);
}
int sxdkl_uart_set_param(sxdkl_param_t *sp)
{
	return sxdkl_uart_request_add(SXDKL_UART_CMD_SET, sp, sizeof(sxdkl_param_t));
}

static int sxdkl_uart_get_param()
{
	return sxdkl_uart_request_add(SXDKL_UART_CMD_GET, NULL, 0);
}

int sxdkl_uart_wifi_stat(wifi_bits *wb)
{
	return sxdkl_uart_request_add(SXDKL_UART_CMD_WIFI_STAT, wb, sizeof(wifi_bits));
}
void sxdkl_change_state_by_pnone(sxdkl_param_t *sp)
{
	memcpy(&sxdkl_stat.func_bits, sp, sizeof(sxdkl_param_t));
}
u_int8_t *sxdkl_get_state_data()
{
	return &sxdkl_stat.func_bits;
}
static int  sxdkl_query_timer(cl_thread_t *t)
{
	cl_thread_t *query_timer = NULL;
	u_int8_t tmp = 0;
	sxdkl_uart_get_param();
	CL_THREAD_TIMER_ON(MASTER, query_timer, sxdkl_query_timer, NULL, TIME_N_MSECOND(SXDKL_QUERY_TIME));
    return 0;
}
void sxdkl_uart_init()
{
	u_int8_t err;
	cl_thread_t *query_timer = NULL;
	SXDKL_INFO("sxdkl_init!!!\n");
	
	memset(&sxdkl_uart, 0, sizeof(sxdkl_uart_t));
	sxdkl_uart.mutext_out =  OSMutexCreate(NST_UART_WRITE_TASK_PRIO, &err);
	if (sxdkl_uart.mutext_out == NULL) {
	    SXDKL_INFO("sxdkl_init OSMutexCreate failed: err=%u\n", err);
	}
	sxdkl_uart.cur_uart_db = &sxdkl_uart.uart_db[0];
	sxdkl_uart.state = SXDKL_NONE;
	//读串口数据到缓存
	ucd_event_add(EVENT_UC_CHIP2UART, sxdkl_uart_callback);
	//处理缓存数据
	ucd_event_add(EVENT_UC_PARSE_UARTDATA_COMMON, sxdkl_uart_cmd_parse);
	BSP_UartSet_irqparsefun(true);
	init_sxdkl_uart_send_list();
	sxdkl_uart.init_done = true;
	CL_THREAD_TIMER_ON(MASTER, query_timer, sxdkl_query_timer, NULL, TIME_N_MSECOND(SXDKL_QUERY_TIME));
	sxdkl_uart_set_param((sxdkl_param_t *)&sxdkl_stat.func_bits);
}


