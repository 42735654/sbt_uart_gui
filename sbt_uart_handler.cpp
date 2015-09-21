#include "sbt_uart_handler.h"

sbt_uart_handler::sbt_uart_handler(QSerialPort::BaudRate buad)
{
    serial = NULL;
    btl = buad;
}

sbt_uart_handler::~sbt_uart_handler()
{
    if (serial){
        delete serial;
    }
}

bool sbt_uart_handler::data_is_cmd()
{
    return false;
}
void sbt_uart_handler::set_arg_by_uart()
{

}
void sbt_uart_handler::cmd_data_parser()
{

}

void sbt_uart_handler::uart_recvie()
{
    while(serial->canReadLine()){
        udata = serial->readLine();
        if (data_is_cmd()){
            set_arg_by_uart();
        }else{
            emit log_to_ui(udata);
        }
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
    connect(serial, SIGNAL(readyRead()), this, SLOT(uart_recvie()));
}
