/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 商贤电烤炉
**  File:    sxdkl_uart.h
**  Author:  wangdawei
**  Date:    09/30/2015
**
**  Purpose:
**    串口协议.
**************************************************************************/


#ifndef SXDKL_UART_H
#define SXDKL_UART_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct{
	u_int8_t recv_suc:1;		//Bit0：数据接收：     0：失败   1：成功 
	u_int8_t pad0:1;		
	u_int8_t reset:1;    	//Bit2：复位命令：0：取消， 1：复位（直接复位清空测试数据，并返回复位结果） 
	u_int8_t wifi_conn:1; 	//Bit3：wifi进入配网：  0：取消， 1：联网；   
	u_int8_t wifi_test:1;	//Bit4：wifi进入厂测：  0：取消， 1：厂测，（结果会在1-3内返回给电脑板）；
	u_int8_t wifi_disbind:1;//Bit5：wifi解除绑定：  0：取消， 1：绑定 ，（结果不会立即返回，需要电脑板等待）
	u_int8_t pad1:2;
}wifi_ctl_bits;

typedef struct {
	u_int8_t func_bits;
	u_int8_t level_bits;
	u_int8_t speed_bits;
	u_int8_t temp;//设定温度：15-32C(90-60F)
	u_int8_t timer_bits;
	u_int8_t mins;//定时分钟：00-59分
}sxdkl_param_t;

typedef struct {
	u_int8_t sync;
	wifi_ctl_bits cmd_ack;
	u_int8_t func_bits;
	u_int8_t level_bits;
	u_int8_t speed_bits;
	u_int8_t temp;//设定温度：15-32C(90-60F)
	u_int8_t timer_bits;
	u_int8_t mins;//定时分钟：00-59分
	u_int8_t ntc_error;
	u_int8_t temp_error;
	u_int8_t work_stat;	//Bit0：电热工作：0-关，1-开 
	u_int8_t pad;
	u_int8_t checksum;
}sxdkl_res_t;

#define WIFI_MODE_STA	1
#define WIFI_MODE_AP	0

extern sxdkl_res_t sxdkl_stat;
extern u_int8_t wifi_mode;

//商贤电烤炉参数设置
extern int sxdkl_uart_set_param(sxdkl_param_t *sp);

//更新缓存状态
extern void sxdkl_change_state_by_pnone(sxdkl_param_t *sp);

//通过app切换wifi模式
extern void sxdkl_ap_to_sta(void);

extern u_int8_t *sxdkl_get_state_data(void);
//初始化函数
extern void sxdkl_uart_init(void);

//配置初始化
extern void sxdkl_config_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SBTWKQ_UART_H */

