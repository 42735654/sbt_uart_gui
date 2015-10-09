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

typedef unsigned char u_int8_t;

class uart_handler:public QObject
{
    Q_OBJECT
public:
    QSerialPort *serial;        //qt的串口类
    QByteArray udata;           //串口读取数据缓存
    QSerialPort::BaudRate btl;  //波特率
    u_int8_t arg[UART_STAT_BUF_LEN];              //项目相关参数，对应到ui中的控件显示
    u_int8_t last_arg[UART_STAT_BUF_LEN];              //备份参数
    enum signal_type{
        ARG_CHANGED = 0,
    };
    enum update_arg_type{
        UI_TO_ARG,
        ARG_TO_UI,
    };
protected:
    int cmd_count;
    cmd_info *cmds;
public:
    uart_handler();
    virtual ~uart_handler();

    virtual int uart_send(u_int8_t *buf, u_int8_t len);              //发送数据
    bool open_serial_port(QString port_name);      //打开串口
    virtual void uart_recvie();                                   //串口数据的接收方法
    void log_to_ui(QString s);
    void set_arg_by_uart(u_int8_t cmd);                       //根据串口数据设置参数
    void uart_cmd_reply_query(int type = 0);

    virtual void init_serial_param() = 0;                      //初始化串口参数，派生类需重载
    virtual bool data_is_cmd() = 0;                           //接收到的是否是串口命令
    virtual void init_self_widgets(int type, widgets_t *w) = 0;
    virtual bool can_read_uart() = 0;
    virtual bool check_sum() = 0;
    virtual u_int8_t get_cmd_from_pkt() = 0;
    virtual u_int8_t *generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len) = 0;
    virtual void update_arg(update_arg_type type) = 0;

signals:
    void s_log_to_ui(QString s);                                  //输出信息到ui
    void signal_for_uart_to_ui(uart_handler::signal_type t);                        //发送信号

public slots:
    void begin_to_recvie();
    void slot_uart_send(u_int8_t *buf, u_int8_t len){uart_send(buf, len);}
};


#endif // UART_HANDLER_H
