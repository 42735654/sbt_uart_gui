#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
class uart_handler:public QObject
{
public:
    QSerialPort *serial;
public:
    uart_handler();
    virtual ~uart_handler() = 0;
    virtual void uart_send(){}
    virtual void uart_recvie(){}
    virtual void init_serial_param(){}
    bool open_serial_port(QString port_name);
};

#endif // UART_HANDLER_H
