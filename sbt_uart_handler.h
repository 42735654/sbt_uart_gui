#ifndef SBT_UART_HANDLER_H
#define SBT_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <uart_handler.h>
class sbt_uart_handler:public uart_handler
{
public:
    sbt_uart_handler();
    ~sbt_uart_handler();
    void uart_recvie();
    void init_serial_param();
public slots:
    void serialRead(){}
};

#endif // SBT_UART_HANDLER_H
