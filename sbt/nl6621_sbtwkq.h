/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ˼�����¿���
**  File:    nl6621_sbtwkq.h
**  Author:  wangdawei
**  Date:    09/16/2015
**
**  Purpose:
**    ͨ�ò���.
**************************************************************************/


#ifndef NL6621_SBTWKQ_H
#define NL6621_SBTWKQ_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
//�豸����
#define DEV_HOST_NAME			"SBTWKQ"

#define HAS_SBTWKQ			
//Ĭ��Flash��СΪ512K
#define DEF_FLASH_SIZE		(2 * 1024 * 1024) //Ĭ��Flash��СΪ2M 

#define UART_BAUDRATE   UART_BAUDRATE_9600

//����һ����������ĳ�ʱʱ��,��λ΢��
#define WAITTIME_UART_SEND (330000)
//����һ������ڵļ��ʱ�䣬��λ΢��
#define CHECK_TIME_UART_SEND (10000)

//���ڲ��⹦��
#define HAS_UART_PT

#define HAS_TT

//ʹ��6621ģ����led��
#define HAS_WIFICARD_LED

//ȥ��printf��ӡ
//#define HAS_NO_PRINTF

//֧�ֶྵ��ѡ��
#define HAS_BOOTLOAD

//Ĭ�ϵ��Լ���
#define	DEBUG_LEVEL	2


/* Type definitions. */


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* NL6621_SBTWKQ_H */

