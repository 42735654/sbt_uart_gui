#ifndef SBT_UI_DEFINE
#define SBT_UI_DEFINE
#include "camlight_type_define.h"
//数组名称不能变，在uart_gui.cpp中用来初始化界面，每一项对应一个label和输入框
status_widgets_info  self_widgets[] = {
    {WTYPE_LABEL_TEXTLINE, GET_BUF_INDEX( sync), "同步码"},
    {WTYPE_LABEL_TEXTLINE, GET_BUF_INDEX( cmd), "命令"},
    {WTYPE_LABEL_TEXTLINE, GET_BUF_INDEX( color), "颜色值"},
    {WTYPE_LABEL_TEXTLINE, GET_BUF_INDEX( light), "亮度值"},
    {WTYPE_LABEL_TEXTLINE, GET_BUF_INDEX( light_param), "总亮度参数"},
};
#endif // SBT_UI_DEFINE

