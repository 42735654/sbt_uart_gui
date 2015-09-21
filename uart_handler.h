#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QByteArray>
class uart_handler:public QObject
{
    Q_OBJECT
public:
    QSerialPort *serial;
    QByteArray udata;
public:
    uart_handler();
    virtual ~uart_handler() = 0;

    virtual int uart_send(char *buf, int len);
    bool open_serial_port(QString port_name);


    virtual void init_serial_param(){}
    virtual bool data_is_cmd(){return false;}
    virtual void set_arg_by_uart(){}
public slots:
    virtual void uart_recvie(){}
signals:
    void log_to_ui(QString s);

};

#endif // UART_HANDLER_H
