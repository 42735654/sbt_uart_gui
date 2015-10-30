#ifndef YJDNQ_UI_DEFINE
#define YJDNQ_UI_DEFINE
#include "config.h"
#include "type_def.h"
#include INCLUDE_FILE(INCLUDE_FILE_PART(DST_PROJECT, _type_define.h))

//数组名称不能变
//控件类型,该状态控件在arg中对应的字节的索引，控件的名称
//如果要解析出某个位来显示，要在arg中未使用的地方重新找一个字节，在串口解析类的update_arg函数中将位解析出来；
status_widgets_info  self_widgets[] = {
    {WTYPE_PUSHBOTTON, PRODUCT_TEST_BOTTON, "产测"},

    {WTYPE_LABEL_TEXTLINE, GET_INDEX(power), "开关"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(set_temp), "设置温度"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(temp_unit), "温度单位"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(anion), "负离子"},

    {WTYPE_LABEL_TEXTLINE, GET_INDEX(heat_level), "功率"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(lock), "童锁"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(house_temp), "室温"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(timer), "定时"},

    {WTYPE_LABEL_TEXTLINE, GET_INDEX(error), "错误码"},
    {WTYPE_LABEL_TEXTLINE, GET_INDEX(wifi_stat), "wifi状态"},

};
#endif

