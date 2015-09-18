/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 思博特温控器
**  File:    uc_sbtwkq.h
**  Author:  wangdawei
**  Date:    09/16/2015
**
**  Purpose:
**    网络通讯.
**************************************************************************/


#ifndef UC_SBTWKQ_H
#define UC_SBTWKQ_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
//#define CAM_DEBUG
//#define CAM_DEBUG_NO_LED
/* Type definitions. */


/* External function declarations. */
extern bool sbtwkq_stat_push(void);
extern void sbtwkq_init(void);
/* Macro API definitions. */

/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UC_SBTWKQ_H */

