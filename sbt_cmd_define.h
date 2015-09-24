#ifndef SBT_CMD_DEFINE
#define SBT_CMD_DEFINE
#include "type_def.h"
#include "sbt_type_define.h"

#define INDEX_8  8
#define REPLY_TYPE_100	100
#define REPLY_TYPE_101	101
#define INDEX_0 0
//命令名称， 命令号， 在参数中占得空间起始地址， 占用长度，参数在报文中的索引，回复类型，回复数据在参数中的索引，回复长度
cmd_info cmd_infos[] = {
    {"工作参数设置", 1, 0, 4, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"特殊参数设置", 2, 4, 8, INDEX_8, REPLY_TYPE_100, INDEX_0 ,sizeof(uart_stat_arg)},
    {"时间设置", 4, 20, 4, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"自动模式参数设置", 5, 24, 24, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"查询状态", 6, 0, 0, INDEX_8, REPLY_TYPE_100, INDEX_0, sizeof(uart_stat_arg)},
    {"wifi状态下发", 7, 0, 0, INDEX_8, REPLY_TYPE_101, INDEX_0, sizeof(uart_stat_arg)},
};
#endif // SBT_CMD_DEFINE

