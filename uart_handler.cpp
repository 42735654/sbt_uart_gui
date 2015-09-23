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
    //    if (recv_pthread->isRunning()){
    //        return;
    //    }
    uart_recvie();
    //    recv_pthread->start();
}

int uart_handler::uart_send(u_int8_t *buf, u_int8_t len)
{
    if (!serial){
        log_to_ui("请先打开一个串口！");
        return 0;
    }
    QString log;
    int has_send_len = serial->write((char *)buf, len);
    serial->flush();
    if (has_send_len != len){
        log.sprintf("uart send error!ask send %d bytes but only send %d bytes", len, has_send_len);
        log_to_ui(log);
    }
    return has_send_len;
}
void uart_handler::uart_recvie()
{
    if (!serial->canReadLine()){
        return;
    }
    udata = serial->readLine();

    if (data_is_cmd()){
        log_to_ui(udata.toHex().data());
        set_arg_by_uart();
        if (memcmp(arg, last_arg, stat_len)){
            memcpy(last_arg, arg, stat_len);
            emit signal_for_uart_to_ui(ARG_CHANGED);
        }
    }else{
        udata.replace('\n', ' ');
        udata.replace('\r', ' ');
        log_to_ui(udata);
    }

}
uart_recv_pthread::uart_recv_pthread(uart_handler *hd)
{
    uhd = hd;
}

void uart_recv_pthread::run()
{
    uhd->uart_recvie();
}
