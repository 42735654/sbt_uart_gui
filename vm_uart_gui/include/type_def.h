#ifndef TYPE_DEF
#define TYPE_DEF
#include <QWidget>
#include "config.h"

typedef unsigned short uint16_t;
typedef unsigned short u_int16_t;
typedef short int16_t;
typedef int int32_t;
typedef unsigned int u_int32_t;
typedef unsigned int uint32_t;
typedef unsigned char u_int8_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

#define MAX_WIDGETS_NUM    70               //最大控件数量

//控件类型定义
#define    WTYPE_PUSHBOTTON                1
#define    WTYPE_CHECKBOX                     2
#define    WTYPE_LABEL_TEXTLINE         3

//调试信息类型
#define LOG_TYPE_INFO   1
#define LOG_TYPE_WARN   2
#define LOG_TYPE_ERROR   3
#define LOG_TYPE_DEBUG   4

#pragma pack (1)
typedef struct{
    u_int8_t widget_type;
    u_int8_t index_in_arg;
    const char *widget_name;
}status_widgets_info;
#pragma pack ()

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

