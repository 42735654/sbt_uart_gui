/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ���͵翾¯
**  File:    sxdkl_uart.h
**  Author:  wangdawei
**  Date:    09/30/2015
**
**  Purpose:
**    ����Э��.
**************************************************************************/


#ifndef SXDKL_UART_H
#define SXDKL_UART_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct{
	u_int8_t recv_suc:1;		//Bit0�����ݽ��գ�     0��ʧ��   1���ɹ� 
	u_int8_t pad0:1;		
	u_int8_t reset:1;    	//Bit2����λ���0��ȡ���� 1����λ��ֱ�Ӹ�λ��ղ������ݣ������ظ�λ����� 
	u_int8_t wifi_conn:1; 	//Bit3��wifi����������  0��ȡ���� 1��������   
	u_int8_t wifi_test:1;	//Bit4��wifi���볧�⣺  0��ȡ���� 1�����⣬���������1-3�ڷ��ظ����԰壩��
	u_int8_t wifi_disbind:1;//Bit5��wifi����󶨣�  0��ȡ���� 1���� ������������������أ���Ҫ���԰�ȴ���
	u_int8_t pad1:2;
}wifi_ctl_bits;

typedef struct {
	u_int8_t func_bits;
	u_int8_t level_bits;
	u_int8_t speed_bits;
	u_int8_t temp;//�趨�¶ȣ�15-32C(90-60F)
	u_int8_t timer_bits;
	u_int8_t mins;//��ʱ���ӣ�00-59��
}sxdkl_param_t;

typedef struct {
	u_int8_t sync;
	wifi_ctl_bits cmd_ack;
	u_int8_t func_bits;
	u_int8_t level_bits;
	u_int8_t speed_bits;
	u_int8_t temp;//�趨�¶ȣ�15-32C(90-60F)
	u_int8_t timer_bits;
	u_int8_t mins;//��ʱ���ӣ�00-59��
	u_int8_t ntc_error;
	u_int8_t temp_error;
	u_int8_t work_stat;	//Bit0�����ȹ�����0-�أ�1-�� 
	u_int8_t pad;
	u_int8_t checksum;
}sxdkl_res_t;

#define WIFI_MODE_STA	1
#define WIFI_MODE_AP	0

extern sxdkl_res_t sxdkl_stat;
extern u_int8_t wifi_mode;

//���͵翾¯��������
extern int sxdkl_uart_set_param(sxdkl_param_t *sp);

//���»���״̬
extern void sxdkl_change_state_by_pnone(sxdkl_param_t *sp);

//ͨ��app�л�wifiģʽ
extern void sxdkl_ap_to_sta(void);

extern u_int8_t *sxdkl_get_state_data(void);
//��ʼ������
extern void sxdkl_uart_init(void);

//���ó�ʼ��
extern void sxdkl_config_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SBTWKQ_UART_H */

