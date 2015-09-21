#include "sbt_uart_handler.h"

sbt_uart_handler::sbt_uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud9600;
    recv_pthread = new uart_pthread_recv(this);
}

sbt_uart_handler::~sbt_uart_handler()
{
    if (serial){
        delete serial;
    }
}

bool sbt_uart_handler::data_is_cmd()
{
    if (udata.at(0) == 0xFF && udata.at(1) == 0xAA){
        return true;
    }else{
        return false;
    }
}

void sbt_uart_handler::set_arg_by_uart()
{
    u_int8_t cmd = udata.at(2);
    switch(cmd){
            default:
                break;
    }
}

void sbt_uart_handler::init_serial_param()
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
