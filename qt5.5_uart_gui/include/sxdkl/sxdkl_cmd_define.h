#ifndef SXDKL_CMD_DEFINE
#define SXDKL_CMD_DEFINE
#include "type_def.h"
#include "sxdkl_type_define.h"

//数组名称不能变，在uart_handler.cpp中用来解析命令
//命令名称， 命令号， 在参数中占得空间起始地址， 占用长度，参数在报文中的索引，回复类型，回复数据在参数中的索引，回复长度
cmd_info cmd_infos[] = {
    {"WIFI状态下发", 1, GET_INDEX_BY_NAME(uart_stat_arg, wb), 2, 8, 0, 0, 13},
    {"状态查询", 2, 0, 0, 0, 0, 0, 13},
    {"参数设置", 3, 2, 6, 2, 0, 0 ,13},

};
#endif // SBT_CMD_DEFINE

