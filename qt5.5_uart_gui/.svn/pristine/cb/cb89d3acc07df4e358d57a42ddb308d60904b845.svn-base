#ifndef SBT_DEFINE_H
#define SBT_DEFINE_H
#include "uart_gui.h"
#include "type_def.h"
#include "config.h"
#define INDEX_8  8
#define SHOW_TIME_TABLE 1
#define REPLY_TYPE_100	100
#define REPLY_TYPE_101	101
#define INDEX_0 0
#define GET_SBT_INDEX(name)   GET_INDEX_BY_NAME(uart_stat_arg, name)
//用来表示下位机的状态数据结构,名字不能变
typedef struct{
    u_int8_t temp;
    u_int8_t wind;
    u_int8_t mode;
    u_int8_t power;

    u_int8_t auto_mode;
    u_int8_t temp_makeup;
    u_int8_t low_temp;
    u_int8_t wind_power;

    u_int8_t temp_rd;
    u_int8_t low_mode;
    u_int8_t max_temp;
    u_int8_t min_temp;

    u_int8_t lock;
    u_int8_t temp_dev;
    u_int8_t broadcast_mode;
    u_int8_t no_money;

    u_int8_t ext_dev_max_temp;
    u_int8_t pad;
    u_int8_t house_temp_int;
    u_int8_t house_temp_dot;

    u_int8_t h;
    u_int8_t m;
    u_int8_t s;
    u_int8_t w;
    u_int8_t time_table_begin;
    u_int8_t time_table[23];
}uart_stat_arg;



#endif // SBT_DEFINE_H
