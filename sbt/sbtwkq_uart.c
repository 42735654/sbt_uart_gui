/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 思博特温控器
**  File:    sbtwkq_uart.c
**  Author:  wangdawei
**  Date:    09/16/2015
**
**  Purpose:
**    串口数据读取.
**************************************************************************/


/* Include files. */
#include "includes.h"
#include "smart_config.h"
#include "ds_proto.h"
#include "uc_dev.h"
#include "qp_uart.h"
#include "uc_dev_event.h"
#include "uc_push.h"
#include "user_config.h"
#include "mu_type.h"
#include "sbtwkq_uart.h"
#include "uc_sbtwkq.h"
#include "lc_fhf_led.h"
/* Macro constant definitions. */

//框架参数

#define MAX_UART_BD_NUM			3
#define UART_HEAD_LEN			5
#define UART_CHECKSUM_LEN		1

//协议相关
#define UART_CMD_INDEX			2
#define UART_DEV_TYPE_INDEX		3
#define SBTWKQ_UART_SYNC1		0xFF
#define SBTWKQ_UART_SYNC2		0xAA

#define SBT_UART_CMD_QUERY			6
#define SBT_UART_CMD_WIFI_STAT		7
#define SBT_UART_CMD_PT				8

#define SBT_UART_CMD_QUERY_REPLY	100
#define SBT_UART_CMD_TEST			101

#define SBTWKQ_UART_QUERY_TIME		2

enum {
	WIFI_STATE_SMARTCONFIG = 1,		/*一键配置中*/
	WIFI_STATE_CONNECT_ROUTER = 2,	/*一键配置成功，处于连接路由器中*/
	WIFI_STATE_CONNECT_SERVER = 3,		/*连接路由器成功，处于连接服务器中*/
	WIFI_STATE_CONNECT_SERVER_COMPLETE = 4,		/*连接服务器成功*/
	WIFI_STATE_MAX
};

//串口接受数据状态
typedef enum {
	US_NONE,
	US_SYNC2,
	US_DATA,
}sbtwkq_us_t;


//串口信息
typedef struct sbtwkq_uart_s{
	bool init_done;
	OS_EVENT *mutext_out;
	sbtwkq_uart_db_t *cur_uart_db;
	sbtwkq_uart_db_t uart_db[MAX_UART_BD_NUM];
    u_int8_t bd_parse_begin;
	u_int8_t state;
	u_int16_t index_read;
	u_int32_t last_input_time;		// 最后一次串口输入的时间
}sbtwkq_uart_t;

typedef struct {
	struct stlc_list_head link;
	u_int8_t len;
}sbtwkq_uart_pkt_t;

static sbtwkq_uart_t sbtwkq_uart;
struct stlc_list_head send_list;
sbtwkq_uart_db_t sbtwkq_last_stat;

extern void serial_active(u_int8_t ch);
static u_int8_t sbtwkq_checksum_calc(u_int8_t *buf, u_int8_t len)
{
	int sum = 0;
	u_int8_t i;
	if (NULL == buf){
		return 0;
	}
	for (i = 0; i < len; i++){
		sum += buf[i];
	}
	return sum & 0xff;
}

static void sbtwkq_uart_input(void)
{
	u_int8_t value;
	u_int8_t checksum;
	
	// 长时间无数据输入，而之前有收到部分报文，做丢弃处理
	if (get_msec() - sbtwkq_uart.last_input_time > 60) {
		if (sbtwkq_uart.state != US_NONE) {
			SBTWKQ_INFO("drop prev uart:stat=%u, idx_read=%u\n", sbtwkq_uart.state, sbtwkq_uart.index_read);
			sbtwkq_uart.index_read = 0;
			sbtwkq_uart.state = US_NONE;
		}
	}

	if ((NST_RD_UART_REG(LSR_OFFSET) & LSR_DR_MASK) == 0) {
		SBTWKQ_INFO("LSR_OFFSET ERROR\r\n");
		return;
	}
	
	value = NST_RD_UART_REG(RBR_OFFSET);

	//激活相关
	serial_active(value);
	ucd_event_call(EVENT_UART_VALUE, &value, sizeof(value));

	//存放数据缓存满了
	if (sbtwkq_uart.cur_uart_db->has_data) {
		SBTWKQ_INFO("sbtwkq uart bd full ... \n");
		goto err;
	}

	sbtwkq_uart.last_input_time = get_msec();

	switch (sbtwkq_uart.state) {
	case US_NONE:
		if (SBTWKQ_UART_SYNC1 == value){
			sbtwkq_uart.state = US_SYNC2;
			sbtwkq_uart.cur_uart_db->data[sbtwkq_uart.index_read++] = value;
		}
		break;
	case US_SYNC2:
		if (SBTWKQ_UART_SYNC2 == value){
			sbtwkq_uart.state = US_DATA;
			sbtwkq_uart.cur_uart_db->data[sbtwkq_uart.index_read++] = value;
		}
		break;	
	case US_DATA:
		// 数据过多，溢出
		if (sbtwkq_uart.index_read >= SBTWKQ_UART_DATA_LEN) {
			goto err;
		}
		sbtwkq_uart.cur_uart_db->data[sbtwkq_uart.index_read++] = value;
		
		if (sbtwkq_uart.index_read == UART_HEAD_LEN){
			sbtwkq_uart.cur_uart_db->data_len = UART_HEAD_LEN + value + UART_CHECKSUM_LEN;
			SBTWKQ_INFO("pkt len is %u\n", sbtwkq_uart.cur_uart_db->data_len);
			break;
		}
		if (sbtwkq_uart.index_read >= sbtwkq_uart.cur_uart_db->data_len){
			goto done;
		}
		break;
	}
	return;

done:
	/*
	//透传不用检测校验和
	checksum = sbtwkq_checksum_calc(sbtwkq_uart.cur_uart_db->data, 
					sbtwkq_uart.cur_uart_db->data_len - 1);
	if (checksum != value)
	{
		SBTWKQ_INFO("checksum error!local calc:%u, in pkt:%u\n", checksum, value);
		goto err;
	}
	*/
	sbtwkq_uart.cur_uart_db->has_data = true;
	value = sbtwkq_uart.cur_uart_db - &sbtwkq_uart.uart_db[0];	
	sbtwkq_uart.cur_uart_db = &sbtwkq_uart.uart_db[(value + 1) % MAX_UART_BD_NUM];

err:
	sbtwkq_uart.index_read  = 0;
	sbtwkq_uart.state = US_NONE;
}

// 串口中断回调函数
static void sbtwkq_uart_callback(int event, void *data, int data_len)
{
	sbtwkq_uart_input();
}
static void uart_send_product_test_cmd()
{
	

}
static void sbtwkq_do_uart_cmd(sbtwkq_uart_db_t *bd)
{
	u_int8_t tmp_devtype;
	u_int8_t cmd = bd->data[UART_CMD_INDEX];
	
	SBTWKQ_INFO("GET UART CMD:%u!\n", cmd);
	
	switch(cmd){
		case SBT_UART_CMD_QUERY_REPLY:
			if (memcmp(sbtwkq_last_stat.data, bd->data, bd->data_len) == 0){
				break;
			}else{
				tmp_devtype = bd->data[UART_DEV_TYPE_INDEX];
				if (tmp_devtype != ucd.product_type){
					ucd.product_type = tmp_devtype;
					conf_dev_set(DEV_PRODUCT_TYPE, &ucd.product_type, sizeof(ucd.product_type));
				}
				memcpy(&sbtwkq_last_stat, bd->data, sizeof(sbtwkq_uart_db_t));
				uc_push_set();
			}
			break;
		case SBT_UART_CMD_TEST:
			uart_send_product_test_cmd();
			break;
		default:
			break;
	}
	
}
static void sbtwkq_uart_cmd_parse(int event, void *data, int data_len)
{
	int i;
	sbtwkq_uart_db_t *bd;
	
	for (i = 0; i < MAX_UART_BD_NUM; i++) {
		bd = &sbtwkq_uart.uart_db[sbtwkq_uart.bd_parse_begin];
		if (!bd->has_data){
			break;
		}
		
		sbtwkq_do_uart_cmd(bd);
		bd->has_data = false;
		sbtwkq_uart.bd_parse_begin = (sbtwkq_uart.bd_parse_begin + 1) % MAX_UART_BD_NUM;
	}
}

/*******************************************************************************
							串口发送指令
********************************************************************************/

static int sbtwkq_uart_send(u_int8_t *data, u_int8_t len)
{
	u_int8_t err;
	int i = 0;
	
	if (!sbtwkq_uart.init_done || !data){
		return -1;
	}
	
	OSMutexPend(sbtwkq_uart.mutext_out, 0, &err);

	for(i = 0; i < sizeof(len); i++){
		BSP_UartPutcPolled(data[i]);
	}

	OSMutexPost(sbtwkq_uart.mutext_out);
	return 0;
}
static int sbtwkq_uart_send_timer(cl_thread_t *t)
{
	sbtwkq_uart_pkt_t *pkt = NULL;
	
	cl_thread_t *send_timer = NULL;

	if (stlc_list_empty(&send_list)){
		return 0;
	}
	
	pkt = (pkt_t *)stlc_list_entry(send_list.next, sbtwkq_uart_pkt_t, link);
	sbtwkq_uart_send(&pkt[1], pkt->len);

	stlc_list_del(&pkt->link);
	cl_free(pkt);
	if (stlc_list_empty(&send_list)) {
		return 0;
	}
	CL_THREAD_TIMER_ON(MASTER, send_timer, sbtwkq_uart_send_timer, NULL, TIME_N_MSECOND(150));

	return 0;
}
void sbtwkq_uart_request_add(u_int8_t *data, u_int8_t len)
{
	cl_thread_t *send_timer = NULL;
	sbtwkq_uart_pkt_t *pkt;

	if (!data){
		return;
	}
	pkt = (sbtwkq_uart_pkt_t *)cl_calloc(sizeof(sbtwkq_uart_pkt_t) + len, 1);
	if (!pkt){
		return;
	}
	memcpy(&pkt[1], data, len);
	pkt->len = len;
	if(stlc_list_empty(&send_list)){
		CL_THREAD_TIMER_ON(MASTER, send_timer, sbtwkq_uart_send_timer, NULL, 0);
	}
	stlc_list_add_tail(&pkt->link, &send_list);
}
static u_int8_t *generate_sbtwkq_uart_cmd(u_int8_t cmd, u_int8_t *param, 
						u_int8_t *param_len)
{
	u_int8_t *cmd_buf = NULL;
	u_int8_t total_len = 8 + param_len + 1;
	
	cmd_buf = cl_calloc(total_len, 1);
	cmd_buf[0] = 0xFF;
	cmd_buf[1] = 0xAA;
	cmd_buf[2] = cmd;
	cmd_buf[4] = param_len;
	memcpy(&cmd_buf[8], param, param_len);
	cmd_buf[total_len - 1] = sbtwkq_checksum_calc(cmd_buf, total_len - 1);
	*param_len = total_len;
	return cmd_buf;
}
static int sbtwkq_uart_query(cl_thread_t *t)
{
	u_int8_t len = 0;
	cl_thread_t *tmp_query_timer = NULL;
	u_int8_t *cmd_uf = generate_sbtwkq_uart_cmd(SBT_UART_CMD_QUERY, NULL, &len);
	sbtwkq_uart_request_add(cmd_uf, len);
	cl_free(cmd_uf);
	CL_THREAD_TIMER_ON(MASTER, tmp_query_timer, sbtwkq_uart_query, NULL, 
			TIME_N_SECOND(SBTWKQ_UART_QUERY_TIME));
	return 0;
}
static void sbtwkq_uart_pt()
{
	u_int8_t len = 0;
	u_int8_t *cmd_uf = generate_sbtwkq_uart_cmd(SBT_UART_CMD_PT, NULL, &len);
	sbtwkq_uart_request_add(cmd_uf, len);
	cl_free(cmd_uf);
}
static void sbtwkq_uart_wifi_stat(u_int8_t stat)
{
	u_int8_t len = 1;
	u_int8_t *cmd_uf = generate_sbtwkq_uart_cmd(SBT_UART_CMD_PT, &stat, &len);
	sbtwkq_uart_request_add(cmd_uf, len);
	cl_free(cmd_uf);
}
static void sbtwkq_led_sync(int event, void *data, int data_len)
{
	u_int8_t status = LED_WS_SMART_CONFIG_CRAZY;
	u_int8_t wifi_state = WIFI_STATE_SMARTCONFIG;
	static u_int8_t last_wifi_state = 0xff;
	
	if (event != EVENT_LED_CHANGED ||
		data_len < 1) {
		return;
	}

	status = *(u_int8_t *)data;

	switch(status) {
	case LED_WS_NOACTIVE:
	case LED_WS_SMART_CONFIG_CRAZY:
	case LED_WS_SMART_CONFIG_SCAN:
		wifi_state = WIFI_STATE_SMARTCONFIG;
		break;
	case LED_WS_WAIT_CONNECTING:
		wifi_state = WIFI_STATE_CONNECT_ROUTER;
		break;
	case LED_WS_WAIT_IP:
	case LED_WS_WAIT_CONNECT_SERVER:
	case LED_WS_CONNECTED_IN_DEVPRODUCT:
		wifi_state = WIFI_STATE_CONNECT_SERVER;
		break;
	case LED_WS_SERVER_CONNECTED:
		wifi_state = WIFI_STATE_CONNECT_SERVER_COMPLETE;
		break;
	}

	if (wifi_state != last_wifi_state) {
		last_wifi_state = wifi_state;
		sbtwkq_uart_wifi_stat(wifi_state);
	}
}
void sbtwkq_uart_init()
{
	u_int8_t err;
	cl_thread_t *tmp_query_timer = NULL;
	memset(&sbtwkq_uart, 0, sizeof(sbtwkq_uart));
	
	SBTWKQ_INFO("sbtwkq_uart_init!\n");
	sbtwkq_uart.mutext_out =  OSMutexCreate(NST_UART_WRITE_TASK_PRIO, &err);
	if (sbtwkq_uart.mutext_out == NULL) {
		SBTWKQ_INFO("OSMutexCreate failed: err=%u\n", err);
	}
	sbtwkq_uart.cur_uart_db = &sbtwkq_uart.uart_db[0];
	sbtwkq_uart.state = US_NONE;
	STLC_INIT_LIST_HEAD(&send_list);
	
	//串口读取回调事件
	ucd_event_add(EVENT_UC_CHIP2UART, sbtwkq_uart_callback);
	ucd_event_add(EVENT_UC_PARSE_UARTDATA_COMMON, sbtwkq_uart_cmd_parse);

	ucd_event_add(EVENT_LED_CHANGED, sbtwkq_led_sync);
	
	CL_THREAD_TIMER_ON(MASTER, tmp_query_timer, sbtwkq_uart_query, NULL, 
			TIME_N_SECOND(SBTWKQ_UART_QUERY_TIME));
	
	BSP_UartSet_irqparsefun(true);
	sbtwkq_uart.init_done = true;
}
