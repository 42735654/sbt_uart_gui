#include "uart_handler.h"

uart_handler::uart_handler()
{
    btl = QSerialPort::Baud115200;
    memset(arg, 0, sizeof(arg));
    memset(last_arg, 0, sizeof(last_arg));
}

uart_handler::~uart_handler()
{

}
void uart_handler::log_to_ui(QString s)
{
    emit s_log_to_ui(s);
}

void uart_handler::uart_recvie()
{
    while(serial->canReadLine()){
        udata = serial->readLine();
        if (data_is_cmd()){
            set_arg_by_uart();
            if (memcmp(arg, last_arg, sizeof(arg))){
                memcpy(arg, last_arg, sizeof(arg));
                emit signal_for_uart_to_ui(ARG_CHANGED);
            }
        }else{
            udata.replace('\n', ' ');
            udata.replace('\r', ' ');
            log_to_ui(udata);
        }
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
    connect(serial, SIGNAL(readyRead()), this, SLOT(begin_to_recvie()));
    return ret;
}
void uart_handler::begin_to_recvie()
{
    if (recv_pthread->isRunning()){
        return;
    }
    recv_pthread->start();
}

int uart_handler::uart_send(u_int8_t *buf, int len)
{
    if (!serial){
        log_to_ui("请先打开一个串口！");
        return 0;
    }
    emit serial->readyRead();

    QString log;
    int has_send_len = serial->write((char *)buf, len);
    if (has_send_len != len){
        log.sprintf("uart send error!ask send %d bytes but only send %d bytes", len, has_send_len);
        log_to_ui(log);
    }
    return has_send_len;
}
uart_recv_pthread::uart_recv_pthread(uart_handler *hd)
{
    uhd = hd;
}

void uart_recv_pthread::run()
{
    uhd->uart_recvie();
}
