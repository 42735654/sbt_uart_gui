#ifndef SBT_UART_HANDLER_H
#define SBT_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <uart_handler.h>
#include <QByteArray>

typedef unsigned char u_int8_t;
typedef struct{
    u_int8_t temp;
    u_int8_t wind;
    u_int8_t mode;
    u_int8_t power;

    u_int8_t auto_node;
    u_int8_t temp_makeup;
    u_int8_t low_temp;
    u_int8_t wind_power;

    u_int8_t temp_rd;
    u_int8_t low_mode;
    u_int8_t max_temp;
    u_int8_t min_temp;

    u_int8_t lock;
    u_int8_t temp_dev;
    u_int8_t broadcast_mode;
    u_int8_t no_money;

    u_int8_t ext_dev_max_temp;
    u_int8_t pad;
    u_int8_t house_temp_int;
    u_int8_t house_temp_dot;

    u_int8_t h;
    u_int8_t m;
    u_int8_t s;
    u_int8_t w;

    u_int8_t time_table[24];
}uart_stat_arg;
#define GET_INDEX_BY_NAME(type, name)     (&((type *)0)->name - (u_int8_t *)((type *)0))
class sbt_uart_handler:public uart_handler
{
public:

    sbt_uart_handler();
    virtual ~sbt_uart_handler();

    void init_serial_param();
    bool data_is_cmd();
    void set_arg_by_uart();
    u_int8_t sbtwkq_checksum_calc(u_int8_t *buf, u_int8_t len);

    u_int8_t *generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len);
};

#endif // SBT_UART_HANDLER_H
