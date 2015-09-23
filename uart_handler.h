#ifndef UART_HANDLER_H
#define UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QByteArray>
#include <QTime>
#include <QThread>
#define SBT_UART_CMD_QUERY			6
#define SBT_UART_CMD_WIFI_STAT		7
#define SBT_UART_CMD_QUERY_REPLY	100
#define SBT_UART_CMD_TEST			101
class uart_handler;
typedef unsigned char u_int8_t;
class uart_recv_pthread:public QThread
{
    Q_OBJECT
public:
    uart_recv_pthread(uart_handler *hd);
    void run();

    uart_handler *uhd;
signals:
    void signal_uart_send(u_int8_t *buf, u_int8_t len);
};


class uart_handler:public QObject
{
    Q_OBJECT
public:
    QSerialPort *serial;        //qt的串口类
    QByteArray udata;           //串口读取数据缓存
    QSerialPort::BaudRate btl;  //波特率
    u_int8_t arg[512];              //项目相关参数，对应到ui中的控件显示
    u_int8_t last_arg[512];              //备份参数
    u_int8_t stat_len;
    uart_recv_pthread *recv_pthread;     //读取数据线程

    enum signal_type{
        ARG_CHANGED = 0,
    };

public:
    uart_handler();
    virtual ~uart_handler();

    virtual int uart_send(u_int8_t *buf, u_int8_t len);              //发送数据
    bool open_serial_port(QString port_name);      //打开串口
    virtual void uart_recvie();                                   //串口数据的接收方法
    void log_to_ui(QString s);
    void set_stat_len(int len){stat_len = len;}

    virtual void init_serial_param(){}                      //初始化串口参数，派生类需重载
    virtual bool data_is_cmd() = 0;                           //接收到的是否是串口命令
    virtual void set_arg_by_uart() = 0;                       //根据串口数据设置参数
    virtual void uart_cmd_reply_query()=0;
    virtual u_int8_t *generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len) = 0;

signals:
    void s_log_to_ui(QString s);                                  //输出信息到ui
    void signal_for_uart_to_ui(uart_handler::signal_type t);                        //发送信号

public slots:
    void begin_to_recvie();
    void slot_uart_send(u_int8_t *buf, u_int8_t len){uart_send(buf, len);}
};


#endif // UART_HANDLER_H
