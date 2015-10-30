#ifndef SBT_CMD_DEFINE
#define SBT_CMD_DEFINE
#include "type_def.h"
#include "camlight_type_define.h"

//数组名称不能变，在uart_handler.cpp中用来解析命令
//命令名称， 命令号， 在参数中占得空间起始地址， 占用长度，参数在报文中的索引，回复类型，回复数据在参数中的索引，回复长度
cmd_info cmd_infos[] = {
    {"参数设置", 1, 0, sizeof(uart_stat_arg), 0, 2, 0, sizeof(uart_stat_arg)},
};
#endif // SBT_CMD_DEFINE

