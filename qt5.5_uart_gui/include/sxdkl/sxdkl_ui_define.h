#ifndef SXDKL_UI_DEFINE
#define SXDKL_UI_DEFINE
#include "sxdkl_type_define.h"
//数组名称不能变，在uart_gui.cpp中用来初始化界面，每一项对应一个label和输入框
status_widgets_info  self_widgets[] = {
    {WTYPE_PUSHBOTTON, PRODUCT_TEST_BOTTON, "产测"},
    {WTYPE_PUSHBOTTON, SET_WIFI_TO_AP, "切换到AP模式"},
    {WTYPE_PUSHBOTTON, WIFI_RESET, "回复出厂"},
    {WTYPE_PUSHBOTTON, BRING_NTC_ERROR, "产生NTC故障"},
    {WTYPE_PUSHBOTTON, CLEAN_NTC_ERROR, "清除NTC故障"},
    {WTYPE_PUSHBOTTON, BRING_WKQ_ERROR, "产生温控器故障"},
    {WTYPE_PUSHBOTTON, CLEAN_WKQ_ERROR, "清除温控器故障"},

    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, power), "开关"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, timer_off), "关机定时器"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, timer_h), "定时-时"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, temp), "温度"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, temp_unit), "温度单位"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, mins), "定时-分"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, fire_level), "火力"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, simulat_wood), "假火"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, anion), "负离子"},

    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, router_suc), "路由器连接成功"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, server_suc), "服务器连接成功"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, wifi_mode), "wifi模式"},

    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, light), "火焰灯光"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX_BY_NAME(uart_stat_arg, temp_check), "温度检测"},

};
#endif // SBT_UI_DEFINE

