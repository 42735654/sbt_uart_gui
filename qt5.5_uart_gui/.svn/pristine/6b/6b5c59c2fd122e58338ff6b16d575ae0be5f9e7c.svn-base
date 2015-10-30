#ifndef YJDNQ_UART_HANDLER_H
#define YJDNQ_UART_HANDLER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QWidget>
#include <QByteArray>

#include "config.h"
#include "type_def.h"
#include "uart_handler.h"
#include INCLUDE_FILE(INCLUDE_FILE_PART(DST_PROJECT, _type_define.h))

#define YJDNQ_UART_CMD_QUERY_REPLY	100
#define YJDNQ_UART_CMD_TEST			101
class yjdnq_uart_handler;
extern yjdnq_uart_handler hd;

class yjdnq_uart_handler:public uart_handler
{
public:
    int uart_cmd_len;
    yjdnq_uart_handler();
    virtual ~yjdnq_uart_handler();

    void init_serial_param();   //初始化项目的串口参数
    bool data_is_cmd();             //根据协议判断是否是命令
    bool check_sum();               //根据需要检测校验和

    bool can_read_uart();
    void init_self_widgets(int type, widgets_t *w);
    u_int8_t get_cmd_from_pkt();    //从收到的串口报文中获取命令号
    u_int8_t yjdnq_checksum_calc(u_int8_t *buf, u_int8_t len);         //项目计算校验和的接口
    u_int8_t *generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len);        //根据协议生成串口报文，用来发送给模组
    void update_arg(update_arg_type type);
public slots:
    void product_test_send();
    void set_wifi_to_ap();
    void wifi_reset();
    void bring_ntc_error();
    void clean_ntc_error();
    void bring_wkq_error();
    void clean_wkq_error();
};
#endif // SBT_UART_HANDLER_H
