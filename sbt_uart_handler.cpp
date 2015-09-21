#include "sbt_uart_handler.h"

sbt_uart_handler::sbt_uart_handler()
{
    serial = NULL;
}

sbt_uart_handler::~sbt_uart_handler()
{

}

bool sbt_uart_handler::data_is_cmd()
{
    return false;
}
void sbt_uart_handler::set_arg_by_uart()
{

}

void sbt_uart_handler::uart_recvie()
{
    while(serial->bytesAvailable()){
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
    serial->setBaudRate(QSerialPort::Baud9600);  //波特率
    serial->setDataBits(QSerialPort::Data8); //数据位
    serial->setParity(QSerialPort::NoParity);    //无奇偶校验
    serial->setStopBits(QSerialPort::OneStop);   //停止位1
    serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
    connect(serial, SIGNAL(readyRead()), this, SLOT(uart_recvie()));
}
