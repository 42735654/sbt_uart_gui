#ifndef YJDNQ_DEFINE_H
#define YJDNQ_DEFINE_H
#include "uart_gui.h"
#include "type_def.h"
#include "config.h"

#define PRODUCT_TEST_BOTTON     1

#define YJDNQ_REPLY_CMD        100

#define PRODUCT_TEST_CMD        101

//用来表示下位机的状态数据结构,名字不能变

typedef struct{
    u_int8_t power;
    u_int8_t set_temp;
    u_int8_t temp_unit;
    u_int8_t anion;
    u_int8_t heat_level;
    u_int8_t lock;
    u_int8_t house_temp;
    u_int8_t timer;
    u_int8_t error;

    //自定义
    u_int8_t wifi_stat;
    u_int8_t  back_house_temp;
}uart_stat_arg;


#define GET_INDEX(name)    GET_INDEX_BY_NAME(uart_stat_arg, name)
#endif // SBT_DEFINE_H
