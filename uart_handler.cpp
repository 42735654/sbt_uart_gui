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
    connect(serial, SIGNAL(readyRead()), this, SLOT(serialRead()));
    return serial->open(QIODevice::ReadWrite);
}

