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
    u_int8_t h;
    u_int8_t m;
    u_int8_t s;
    u_int8_t w;
}sbt_arg;
class sbt_uart_handler:public uart_handler
{
public:
    sbt_arg arg;
    sbt_uart_handler(QSerialPort::BaudRate buad);
    ~sbt_uart_handler();

    void init_serial_param();
    bool data_is_cmd();
    void set_arg_by_uart();
    void cmd_data_parser();
public slots:
    void uart_recvie();
};

#endif // SBT_UART_HANDLER_H
