/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 思博特温控器
**  File:    nl6621_sbtwkq.h
**  Author:  wangdawei
**  Date:    09/16/2015
**
**  Purpose:
**    通用参数.
**************************************************************************/


#ifndef NL6621_SBTWKQ_H
#define NL6621_SBTWKQ_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
//设备名称
#define DEV_HOST_NAME			"SBTWKQ"

#define HAS_SBTWKQ			
//默认Flash大小为512K
#define DEF_FLASH_SIZE		(2 * 1024 * 1024) //默认Flash大小为2M 

#define UART_BAUDRATE   UART_BAUDRATE_9600

//发送一条串口命令的超时时间,单位微妙
#define WAITTIME_UART_SEND (330000)
//发送一次命令串口的检测时间，单位微妙
#define CHECK_TIME_UART_SEND (10000)

//串口产测功能
#define HAS_UART_PT

#define HAS_TT

//使用6621模块板的led灯
#define HAS_WIFICARD_LED

//去掉printf打印
//#define HAS_NO_PRINTF

//支持多镜像选择
#define HAS_BOOTLOAD

//默认调试级别
#define	DEBUG_LEVEL	2


/* Type definitions. */


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* NL6621_SBTWKQ_H */

