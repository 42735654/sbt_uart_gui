#include "sbt_uart_handler.h"

sbt_uart_handler::sbt_uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud9600;
    recv_pthread = new uart_recv_pthread(this);
    set_stat_len(sizeof(uart_stat_arg));

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
    if (data[0] == 0xFF && data[1] == 0xAA && data[3] == 0&& udata.length() >= 8 && data[2] != 128){
        qDebug("get uart cmd:%u", data[2]);
        return true;
    }else{
        return false;
    }
}

void sbt_uart_handler::set_arg_by_uart()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    u_int8_t cmd = data[2];
    if (cmd == 128){
        return;
    }
    u_int8_t c = sbtwkq_checksum_calc(data, 8 + data[4]);
    u_int8_t b =  data[8 + data[4]];
//    qDebug("%u---%u", c, b);
    if (c != b){
        qDebug("check sum error!");
        return;
    }


    switch(cmd){
        case SBT_UART_CMD_QUERY:
            uart_cmd_reply_query();
            break;
        case 1:
            memcpy(&arg[0], &data[8], data[4]);
            uart_cmd_reply_query();
            break;
        case 2:
            memcpy(&arg[4], &data[8], data[4]);
            uart_cmd_reply_query();
            break;
        case 4:
            memcpy(&arg[20], &data[8], data[4]);
            break;
        case 5:
        memcpy(&arg[24], &data[8], data[4]);
        break;
        default:
            break;
    }
//    for (int i = 1; i < 8 + data[4] + 1; i++){
//        qDebug("data[%d]=%u", i, data[i]);
//    }
}
void sbt_uart_handler::uart_cmd_reply_query()
{
    u_int8_t len = stat_len;
    u_int8_t *pkt;
    pkt = generate_uart_reply_pkt(SBT_UART_CMD_QUERY_REPLY, arg, &len);
    uart_send(pkt, len);
    free(pkt);
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
//        qDebug("generate_sbtwkq_uart_cmd\n");
        cmd_buf = (u_int8_t *)calloc(total_len, 1);

        cmd_buf[0] = 0xFF;
        cmd_buf[1] = 0xAA;
        cmd_buf[2] = cmd;
        cmd_buf[4] = *len;
        if (param){
            memcpy(&cmd_buf[8], param, *len);
        }
        cmd_buf[total_len - 1] = sbtwkq_checksum_calc(cmd_buf, total_len - 1);
//        qDebug("checksum=%u", cmd_buf[total_len - 1]);
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
