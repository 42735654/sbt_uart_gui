#include "sbt_uart_handler.h"

sbt_uart_handler::sbt_uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud9600;
    show_table = new QPushButton("显示时间表");
    connect(show_table, SIGNAL(clicked(bool)), this, SLOT(show_time_table());
}

sbt_uart_handler::~sbt_uart_handler()
{
    if (serial){
        delete serial;
    }
}

bool sbt_uart_handler::data_is_cmd()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    QString s = QTime::currentTime().toString();
    if (data[0] == 0xFF && data[1] == 0xAA && data[3] == 0&& udata.length() >= 8 && data[2] != 128){
        qDebug("[%s]get uart cmd:%u", s.toLatin1().data(), data[2]);
        return true;
    }else{
        return false;
    }
}
bool sbt_uart_handler::check_sum()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    u_int8_t c = sbtwkq_checksum_calc(data, 8 + data[4]);
    u_int8_t b =  data[8 + data[4]];
#ifdef DEBUG_LOG
    qDebug("%u---%u", c, b);
#endif
    if (c != b){
        qDebug("check sum error!");
        return false;
    }else{
        return true;
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
u_int8_t *sbt_uart_handler::generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len)
{
        u_int8_t *cmd_buf = NULL;
        u_int8_t total_len = 8 + *len + 1;
        cmd_buf = (u_int8_t *)calloc(total_len, 1);

        cmd_buf[0] = 0xFF;
        cmd_buf[1] = 0xAA;
        cmd_buf[2] = cmd;
        cmd_buf[4] = *len;
        if (param){
            memcpy(&cmd_buf[8], param, *len);
        }
        cmd_buf[total_len - 1] = sbtwkq_checksum_calc(cmd_buf, total_len - 1);
        *len = total_len;
        return cmd_buf;
}
u_int8_t sbt_uart_handler::sbtwkq_checksum_calc(u_int8_t *buf, u_int8_t len)
{
    int sum = 0;
    u_int8_t i;
    if (NULL == buf){
        return 0;
    }
    for (i = 0; i < len; i++){
        sum += buf[i];
    }
    return sum & 0xff;
}
u_int8_t sbt_uart_handler::get_cmd_from_pkt()
{
    return udata.data()[2];
}
