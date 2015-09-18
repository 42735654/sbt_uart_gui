#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
class uart_handler:public QSerialPort
{
public:
    uart_handler();
    virtual ~uart_handler() = 0;
    virtual void uart_send(){}
    virtual void uart_recvie(){}
};

#endif // UART_HANDLER_H
