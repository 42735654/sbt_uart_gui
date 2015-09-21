#ifndef SBT_UART_HANDLER_H
#define SBT_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <uart_handler.h>
#include <QByteArray>
typedef unsigned char u_int8_t;
class sbt_uart_handler:public uart_handler
{
public:

    sbt_uart_handler();
    ~sbt_uart_handler();

    void init_serial_param();
    bool data_is_cmd();
    void set_arg_by_uart();
public slots:
    void serialRead(){}
    void uart_recvie();
};

#endif // SBT_UART_HANDLER_H
