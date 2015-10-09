#ifndef TYPE_DEF
#define TYPE_DEF
#include <QWidget>
#include "config.h"
#define MAX_WIDGETS_NUM    50
#define UART_STAT_BUF_LEN       512
#define GET_INDEX_BY_NAME(type, name)     ((u_int8_t *)&((type *)0)->name - (u_int8_t *)((type *)0))
#define SHOW_TIME_TABLE        -1


typedef unsigned char u_int8_t;
//命令名称， 命令号， 在参数中占得空间起始地址， 占用长度，参数在报文中的索引，回复类型，回复数据在参数中的索引，回复长度
typedef struct{
    const char* cmd_name;
    u_int8_t cmd;
    u_int8_t arg_index;
    u_int8_t arg_len;
    u_int8_t pkt_index;
    int reply_type;
    u_int8_t reply_index;
    u_int8_t reply_len;
}cmd_info;

typedef struct{
    int index_in_arg;
    const char *widget_name;
}status_widgets_info;

typedef struct{
    QWidget *label;
    QWidget *lineedit;
    QWidget *self_widget;
    status_widgets_info *swi;
}widgets_t;

//状态控件列表
typedef struct{
    widgets_t swis[MAX_WIDGETS_NUM];
    int swi_count;
}widgets_list;

#endif // TYPE_DEF

