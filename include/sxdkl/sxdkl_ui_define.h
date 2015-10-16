#ifndef SXDKL_UI_DEFINE
#define SXDKL_UI_DEFINE
#include "sxdkl_type_define.h"
//数组名称不能变，在uart_gui.cpp中用来初始化界面，每一项对应一个label和输入框
status_widgets_info  self_widgets[] = {
    {PRODUCT_TEST_BOTTON, "产测"},
    {SET_WIFI_TO_AP, "切换到AP模式"},
    {WIFI_RESET, "回复出厂"},
    {GET_INDEX_BY_NAME(uart_stat_arg, power), "开关"},
    {GET_INDEX_BY_NAME(uart_stat_arg, timer_off), "关机定时器"},
    {GET_INDEX_BY_NAME(uart_stat_arg, timer_h), "定时-时"},
    {GET_INDEX_BY_NAME(uart_stat_arg, temp), "温度"},
    {GET_INDEX_BY_NAME(uart_stat_arg, temp_unit), "温度单位"},
    {GET_INDEX_BY_NAME(uart_stat_arg, mins), "定时-分"},
    {GET_INDEX_BY_NAME(uart_stat_arg, fire_level), "火力"},
    {GET_INDEX_BY_NAME(uart_stat_arg, simulat_wood), "假火"},
    {GET_INDEX_BY_NAME(uart_stat_arg, anion), "负离子"},

    {GET_INDEX_BY_NAME(uart_stat_arg, router_suc), "路由器连接成功"},
    {GET_INDEX_BY_NAME(uart_stat_arg, server_suc), "服务器连接成功"},
};
#endif // SBT_UI_DEFINE

