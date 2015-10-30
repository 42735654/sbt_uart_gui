#ifndef SBT_DEFINE_H
#define SBT_DEFINE_H
#include "uart_gui.h"
#include "type_def.h"
#include "config.h"

#define GET_BUF_INDEX(name)   GET_INDEX_BY_NAME(uart_stat_arg, name)
//用来表示下位机的状态数据结构,名字不能变
typedef struct{
    u_int8_t sync;
    u_int8_t cmd;
    u_int8_t color;
    u_int8_t light;
    u_int8_t light_param;
}uart_stat_arg;



#endif // SBT_DEFINE_H
