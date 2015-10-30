#ifndef TYPE_DEF
#define TYPE_DEF
#include <QWidget>
#include "config.h"
typedef unsigned char u_int8_t;

#define __LINK_LABEL__(a, b)    a##b
#define INCLUDE_FILE_PART(a, b)  __LINK_LABEL__(a, b)

#define __INCLUDE_FILE__(s)         #s
#define INCLUDE_FILE(s) __INCLUDE_FILE__(s)


#define MAX_WIDGETS_NUM    50
#define UART_STAT_BUF_LEN       512


#define GET_INDEX_BY_NAME(type, name)     ((u_int8_t *)&((type *)0)->name - (u_int8_t *)((type *)0))

//控件类型定义
#define    WTYPE_PUSHBOTTON                1
#define    WTYPE_CHECKBOX                     2
#define    WTYPE_LABEL_TEXTLINE         3

//调试信息类型
#define LOG_TYPE_INFO   1
#define LOG_TYPE_WARN   2
#define LOG_TYPE_ERROR   3
#define LOG_TYPE_DEBUG   4

//简单宏定义
#define CMD(n)   n
#define ARG_INDEX(n) n
#define PKT_INDEX(n) n
#define USE_LEN(n)  n
#define NO_USE       0


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
    int widget_type;
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

