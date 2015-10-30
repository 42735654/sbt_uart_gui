#ifndef SBT_UART_HANDLER_H
#define SBT_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <uart_handler.h>
#include <QByteArray>
#include "config.h"
#include "type_def.h"
#include "camlight_type_define.h"
#include <QWidget>
class camlight_uart_handler;
extern camlight_uart_handler hd;

class camlight_uart_handler:public uart_handler
{
public:
    camlight_uart_handler();
    virtual ~camlight_uart_handler();

    void init_serial_param();   //初始化项目的串口参数
    bool data_is_cmd();             //根据协议判断是否是命令
    bool check_sum();               //根据需要检测校验和
    bool can_read_uart();
    void init_self_widgets(int type, widgets_t *w);
    u_int8_t get_cmd_from_pkt();    //从收到的串口报文中获取命令号
    u_int8_t *generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len);        //根据协议生成串口报文，用来发送给模组
    void update_arg(update_arg_type type){}
};

#endif // SBT_UART_HANDLER_H
