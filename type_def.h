#ifndef TYPE_DEF
#define TYPE_DEF
#include <QLabel>
#include <QLineEdit>
#include "config.h"
#define MAX_WIDGETS_NUM    50
#define GET_INDEX_BY_NAME(type, name)     (&((type *)0)->name - (u_int8_t *)((type *)0))

typedef unsigned char u_int8_t;
typedef struct{
    const char* cmd_name;
    u_int8_t cmd;
    u_int8_t arg_index;
    u_int8_t arg_len;
    int reply_type;
}cmd_info;

typedef struct{
    int index_in_arg;
    const char *widget_name;
}status_widgets_info;

typedef struct{
    QLabel *label;
    QLineEdit *lineedit;
    status_widgets_info *swi;
}widgets_t;

typedef struct{
    widgets_t swis[MAX_WIDGETS_NUM];
    int swi_count;
}widgets_list;
#endif // TYPE_DEF

