#ifndef SXDKL_DEFINE_H
#define SXDKL_DEFINE_H
#include "uart_gui.h"
#include "type_def.h"
#include "config.h"
#define INDEX_8  8
#define REPLY_TYPE_100	100
#define REPLY_TYPE_101	101
#define INDEX_0 0
#define GET_SBT_INDEX(name)   GET_INDEX_BY_NAME(uart_stat_arg, name)


#define SET_WIFI_TO_AP     1
#define WIFI_RESET     3
#define BRING_NTC_ERROR       4
#define CLEAN_NTC_ERROR       5
#define BRING_WKQ_ERROR       6
#define CLEAN_WKQ_ERROR      7
#define PRODUCT_TEST_BOTTON     8
//用来表示下位机的状态数据结构,名字不能变
typedef struct{
    u_int8_t recv_suc:1;		//Bit0：数据接收：     0：失败   1：成功
    u_int8_t pad0:1;
    u_int8_t reset:1;    	//Bit2：复位命令：0：取消， 1：复位（直接复位清空测试数据，并返回复位结果）
    u_int8_t wifi_conn:1; 	//Bit3：wifi进入配网：  0：取消， 1：联网；
    u_int8_t wifi_test:1;	//Bit4：wifi进入厂测：  0：取消， 1：厂测，（结果会在1-3内返回给电脑板）；
    u_int8_t wifi_disbind:1;//Bit5：wifi解除绑定：  0：取消， 1：绑定 ，（结果不会立即返回，需要电脑板等待）
    u_int8_t pad1:2;
}wifi_ctl_bits;

typedef struct{
    u_int8_t router_stat:1;		//Bit0：路由连接状态：0：disconnect   1：connect;
    u_int8_t net_stat:1;		//Bit1：网络连接状态：0：disconnect   1：connect;
    u_int8_t reset_suc:1;    	//Bit2：复位成功状态： 0：失败，1：成功；
    u_int8_t wifi_result:1; 	//Bit3：wifi配网结果： 0：未直连；1：成功；
    u_int8_t wifi_test_res:1;	//Bit4: wifi厂测结果： 0：未直连；1：成功
    u_int8_t wifi_disbind_res:1;//Bit5：wifi解除绑定结果： 0：未直连；1：成功
    u_int8_t wifi_mode:1;
    u_int8_t pad:1;
}wifi_bits;
typedef struct {
    u_int8_t pad0 : 1;
    u_int8_t pad1: 1;
    u_int8_t light : 1;
    u_int8_t temp_check: 1;	//bit3,灯光开关
    u_int8_t temp_unit : 1;    //bit 4,0-C，1-F
    u_int8_t anion : 1; 		//bit5, 负离子,0-关，1-开
    u_int8_t lock : 1;		//bit6, 童锁 ，0-关，1-开
    u_int8_t power : 1;		//bit7, 开关，0-关，1-开
}func_bits;

typedef struct {
    u_int8_t simulat_wood : 4;//Bit3-0：假柴:00:OFF;01:L1;02:L2-------09:L9.
    u_int8_t fire_level : 4;//Bit7-4：火焰:00:OFF;01:L1;02:L2-------09:L9.
}level_bits;

typedef struct {
    u_int8_t speed_level : 4;
    u_int8_t pad1 : 4;
}speed_bits;

typedef struct {
    u_int8_t hour : 5;
    u_int8_t pad:1;
    u_int8_t off:1;
    u_int8_t on:1;
}timer_bits;
typedef struct{
    u_int8_t sync;
    wifi_ctl_bits cmd_ack;
    func_bits fb;
    level_bits lb;

    speed_bits sb;
    u_int8_t temp;//设定温度：15-32C(90-60F)
    timer_bits tb;
    u_int8_t mins;//定时分钟：00-59分

    u_int8_t ntc_error;
    u_int8_t temp_error;
    u_int8_t work_stat;	//Bit0：电热工作：0-关，1-开
    u_int8_t pad0;

    u_int8_t checksum;

    wifi_bits wb;
    u_int8_t signal;

    u_int8_t timer_on;
    u_int8_t timer_off;
    u_int8_t timer_h;
    u_int8_t power;
    u_int8_t fire_level;
    u_int8_t anion;
    u_int8_t simulat_wood;
    u_int8_t temp_unit;

    u_int8_t router_suc;
    u_int8_t server_suc;
    u_int8_t wifi_mode;
    u_int8_t light;
    u_int8_t temp_check;
}uart_stat_arg;



#endif // SBT_DEFINE_H
