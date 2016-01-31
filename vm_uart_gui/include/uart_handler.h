#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QByteArray>
#include <QTime>
#include <QThread>
#include "config.h"
#include "type_def.h"
class uart_handler
{
public:
    QSerialPort *serial;        //qt的串口类
    int btl;
public:
    uart_handler();
    virtual ~uart_handler();
    virtual int uart_send(u_int8_t *buf, int len);              //发送数据
    bool open_serial_port(QString port_name);      //打开串口
    void close_serial();
    void init_serial_param();                      //初始化串口参数，派生类需重载
    void slot_uart_send(u_int8_t *buf, u_int8_t len){
        uart_send(buf, len);
    }
};


#endif // UART_HANDLER_H
