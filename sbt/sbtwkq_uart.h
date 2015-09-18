/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 思博特温控器
**  File:    sbtwkq_uart.h
**  Author:  wangdawei
**  Date:    09/16/2015
**
**  Purpose:
**    串口协议.
**************************************************************************/


#ifndef SBTWKQ_UART_H
#define SBTWKQ_UART_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define SBTWKQ_DEBUG				1
#define SBTWKQ_UART_DATA_LEN	128

/* Type definitions. */
//接收数据存储结构
typedef struct {
	u_int8_t has_data;
	u_int8_t data_len;
	u_int8_t data[SBTWKQ_UART_DATA_LEN];
} sbtwkq_uart_db_t;


/* External function declarations. */
extern sbtwkq_uart_db_t sbtwkq_last_stat;
extern void sbtwkq_uart_request_add(u_int8_t * data, u_int8_t len);
extern void sbtwkq_uart_init(void);
/* Macro API definitions. */

#ifdef SBTWKQ_DEBUG
	#define SBTWKQ_INFO	printf
#else
	#define SBTWKQ_INFO	INFO
#endif

/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SBTWKQ_UART_H */

