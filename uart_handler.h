#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
class uart_handler
{
public:
    uart_handler();
    virtual uart_send();
    virtual uart_recvie();
};

#endif // UART_HANDLER_H
