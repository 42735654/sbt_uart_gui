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
    u_int8_t arg[256];              //项目相关参数，对应到ui中的控件显示
    u_int8_t last_arg[256];              //备份参数
        int btl;
public:
    uart_handler();
    virtual ~uart_handler();
    virtual int uart_send(u_int8_t *buf, u_int8_t len);              //发送数据
    bool open_serial_port(QString port_name);      //打开串口
    void close_serial();
    void init_serial_param();                      //初始化串口参数，派生类需重载
    void slot_uart_send(u_int8_t *buf, u_int8_t len){
        uart_send(buf, len);
    }
};


#endif // UART_HANDLER_H
