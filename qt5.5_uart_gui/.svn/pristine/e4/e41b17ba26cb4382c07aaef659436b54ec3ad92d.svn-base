#ifndef YJDNQ_CMD_DEFINE
#define YJDNQ_CMD_DEFINE
#include "type_def.h"
#include "config.h"
#include INCLUDE_FILE(INCLUDE_FILE_PART(DST_PROJECT, _type_define.h))
//数组名称不能变，在uart_handler.cpp中用来解析命令
//命令名称， 命令号， 在参数中占得空间起始地址， 占用长度，参数在报文中的索引，回复类型，回复数据在参数中的索引，回复长度
cmd_info cmd_infos[] = {
    {"状态查询", CMD(2), NO_USE, NO_USE, NO_USE, CMD(100),ARG_INDEX(0) ,USE_LEN(9)},

    {"WIFI状态下发", CMD(3), GET_INDEX(wifi_stat), USE_LEN(1), PKT_INDEX(5), CMD(100), ARG_INDEX(0) ,USE_LEN(9)},

    {"参数设置", CMD(1), ARG_INDEX(0), USE_LEN(7), PKT_INDEX(2), CMD(100), ARG_INDEX(0) ,USE_LEN(9)},

    {"产测成功", CMD(4), NO_USE, NO_USE, NO_USE, CMD(100), ARG_INDEX(0) ,USE_LEN(9)},
};
#endif // SBT_CMD_DEFINE

