#ifndef SBT_UI_DEFINE
#define SBT_UI_DEFINE
#include "sbt_type_define.h"
//数组名称不能变，在uart_gui.cpp中用来初始化界面，每一项对应一个label和输入框
status_widgets_info  self_widgets[] = {
    {GET_SBT_INDEX( temp), "温度"},
    {GET_SBT_INDEX(wind), "风速"},
    {GET_SBT_INDEX(mode), "模式"},
    {GET_SBT_INDEX(power), "开关机"},

    {GET_SBT_INDEX(auto_mode), "自动模式"},
    {GET_SBT_INDEX(temp_makeup), "温度补偿值"},
    {GET_SBT_INDEX(low_temp), "低温保护温度"},
    {GET_SBT_INDEX(wind_power), "阀关模式控制"},

    {GET_SBT_INDEX(temp_rd), "温控器回差温度"},
    {GET_SBT_INDEX(low_mode), "低温保护模式"},
    {GET_SBT_INDEX(max_temp), "限制制热最高温"},
    {GET_SBT_INDEX(min_temp), "限制制冷最低温"},

    {GET_SBT_INDEX(house_temp_int), "房间温度整数"},
    {GET_SBT_INDEX(house_temp_dot), "房间温度小数"},

    {GET_SBT_INDEX(h), "时"},
    {GET_SBT_INDEX(m), "分"},
    {GET_SBT_INDEX(s), "秒"},
    {GET_SBT_INDEX(w), "星期"},
    {100, "WIFI状态"},
};
#endif // SBT_UI_DEFINE

