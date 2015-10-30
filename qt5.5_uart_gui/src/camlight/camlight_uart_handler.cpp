#include "camlight_uart_handler.h"
camlight_uart_handler hd;
camlight_uart_handler::camlight_uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud4800;
}

camlight_uart_handler::~camlight_uart_handler()
{
    if (serial){
        delete serial;
    }
}

bool camlight_uart_handler::data_is_cmd()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    if (data[0] == 0xAA){
        return true;
    }else{
        return false;
    }
}
bool camlight_uart_handler::check_sum()
{
    return true;
}

void camlight_uart_handler::init_serial_param()
{
    if (serial == NULL){
        return;
    }
    serial->setBaudRate(btl);  //波特率
    serial->setDataBits(QSerialPort::Data8); //数据位
    serial->setParity(QSerialPort::NoParity);    //无奇偶校验
    serial->setStopBits(QSerialPort::OneStop);   //停止位1
    serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
}
u_int8_t *camlight_uart_handler::generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len)
{
        u_int8_t *cmd_buf = NULL;
        cmd_buf = (u_int8_t *)calloc(5, 1);

        memcpy(cmd_buf, arg, 5);
        cmd_buf[1] &= 0x0f;
        cmd_buf[1] += cmd << 2;
        *len = 5;
        return cmd_buf;
}
bool camlight_uart_handler::can_read_uart()
{
    if (serial->bytesAvailable() < 5){
        log_to_ui("<5");
        return false;
    }else{
        return true;
    }
}

u_int8_t camlight_uart_handler::get_cmd_from_pkt()
{
    return udata.data()[1] >> 4;
}
void camlight_uart_handler::init_self_widgets(int type, widgets_t *w)
{

}
