#ifndef SBT_UART_HANDLER_H
#define SBT_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <uart_handler.h>
#include <QByteArray>
#include "config.h"
#include "type_def.h"
#define SBT_UART_CMD_WORK_STAT_SET  1
#define SBT_UART_CMD_SPECIAL_SET  2
#define SBT_UART_CMD_TIME_SET  4
#define SBT_UART_CMD_AUTO_SET  5
#define SBT_UART_CMD_QUERY			6
#define SBT_UART_CMD_WIFI_STAT		7
#define SBT_UART_CMD_QUERY_REPLY	100
#define SBT_UART_CMD_TEST			101


class sbt_uart_handler:public uart_handler
{
public:
    sbt_uart_handler();
    virtual ~sbt_uart_handler();

    void init_serial_param();
    bool data_is_cmd();
    void set_arg_by_uart();

    u_int8_t sbtwkq_checksum_calc(u_int8_t *buf, u_int8_t len);
    void uart_cmd_reply_query(int type = -1);
    u_int8_t *generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len);
};

#endif // SBT_UART_HANDLER_H
