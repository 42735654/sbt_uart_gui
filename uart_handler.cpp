#include "uart_handler.h"

uart_handler::uart_handler()
{

}

uart_handler::~uart_handler()
{

}
bool uart_handler::open_serial_port(QString port_name)
{
    if(serial){
        serial->close();
        delete serial;
    }
    serial  = new QSerialPort(port_name);
    return serial->open(QIODevice::ReadWrite);
}

int uart_handler::uart_send(char *buf, int len)
{
    int has_send_len = serial->write(buf, len);
    if (has_send_len != len){
        qDebug("uart send error!ask send %d bytes but only send %d bytes", len, has_send_len);
    }
    return has_send_len;
}
