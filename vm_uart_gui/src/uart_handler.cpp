#include "config.h"
#include "type_def.h"
#include "uart_handler.h"
#include "uart_gui_vm.h"
uart_handler::uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud9600;
    memset(arg, 0, sizeof(arg));
    memcpy(last_arg, arg, sizeof(arg));
}

uart_handler::~uart_handler()
{
    if (serial){
        delete serial;
    }
}
void uart_handler::close_serial()
{
    if(serial){
        serial->close();
        delete serial;
        serial = NULL;
    }
}
bool uart_handler::open_serial_port(QString port_name)
{
    if(serial){
        serial->close();
        delete serial;
    }
    serial  = new QSerialPort(port_name);
    int ret = serial->open(QIODevice::ReadWrite);
    return ret;
}

int uart_handler::uart_send(u_int8_t *buf, int len)
{
    if (!serial){
        ERROR("请先打开一个串口！");
        return 0;
    }
    int has_send_len = serial->write((char *)buf, len);
    serial->flush();

    if (has_send_len != len){
        ERROR("uart_send：发送字节数错误！");
    }
    QByteArray s;
    s.append((char *)buf, len);
    DEBUG("串口发送数据(len=%d):\n%s", len, s.toHex().data());
    return has_send_len;
}

void uart_handler::init_serial_param()
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
