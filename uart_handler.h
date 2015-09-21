#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QByteArray>
#include <QTime>
#include "uart_recv_pthread.h"
class uart_pthread_recv;
typedef unsigned char u_int8_t;
class uart_handler:public QObject
{
    Q_OBJECT
public:
    QSerialPort *serial;        //qt的串口类
    QByteArray udata;           //串口读取数据缓存
    QSerialPort::BaudRate btl;  //波特率
    u_int8_t arg[512];              //项目相关参数，对应到ui中的控件显示
    u_int8_t last_arg[512];              //备份参数
    uart_pthread_recv *recv_pthread;     //读取数据线程

    enum signal_type{
        ARG_CHANGED = 0,
    };

public:
    uart_handler();
    virtual ~uart_handler();

    virtual int uart_send(char *buf, int len);              //发送数据
    bool open_serial_port(QString port_name);      //打开串口
    void uart_recvie();                                   //串口数据的接收方法
    void log_to_ui(QString s);

    virtual void init_serial_param(){}                      //初始化串口参数，派生类需重载
    virtual bool data_is_cmd()=0;                           //接收到的是否是串口命令
    virtual void set_arg_by_uart()=0;                       //根据串口数据设置参数

signals:
    void s_log_to_ui(QString s);                                  //输出信息到ui
    void signal_for_uart_to_ui(uart_handler::signal_type t);                        //发送信号

public slots:
    void begin_to_recvie();
};

#endif // UART_HANDLER_H
