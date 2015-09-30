#ifndef SBT_CMD_DEFINE
#define SBT_CMD_DEFINE
#include "type_def.h"
#include "sxdkl_type_define.h"

//数组名称不能变，在uart_handler.cpp中用来解析命令
//命令名称， 命令号， 在参数中占得空间起始地址， 占用长度，参数在报文中的索引，回复类型，回复数据在参数中的索引，回复长度
cmd_info cmd_infos[] = {
    {"工作参数设置", 1, 0, 4, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"特殊参数设置", 2, 4, 8, INDEX_8, REPLY_TYPE_100, INDEX_0 ,sizeof(uart_stat_arg)},
    {"时间设置", 4, 20, 4, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"自动模式参数设置", 5, 24, 24, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"查询状态", 6, 0, 0, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"wifi状态下发", 7, 100, 1, INDEX_8, -1, INDEX_0, sizeof(uart_stat_arg)},
};
#endif // SBT_CMD_DEFINE

