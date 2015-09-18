#ifndef SBT_UART_HANDLER_H
#define SBT_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>

class sbt_uart_handler:public QSerialPort
{
public:
    sbt_uart_handler();

    uart_send();
    uart_recvie();
};

#endif // SBT_UART_HANDLER_H
