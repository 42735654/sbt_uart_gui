#include "sbt_uart_handler.h"

sbt_uart_handler::sbt_uart_handler()
{
    serial = NULL;
}

sbt_uart_handler::~sbt_uart_handler()
{

}
void sbt_uart_handler::uart_send()
{

}

void sbt_uart_handler::uart_recvie()
{

}


void sbt_uart_handler::init_serial_param()
{
    if (serial == NULL){
        return;
    }
    serial->setBaudRate(QSerialPort::Baud9600);  //波特率
    serial->setDataBits(QSerialPort::Data8); //数据位
    serial->setParity(QSerialPort::NoParity);    //无奇偶校验
    serial->setStopBits(QSerialPort::OneStop);   //停止位1
    serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
}
