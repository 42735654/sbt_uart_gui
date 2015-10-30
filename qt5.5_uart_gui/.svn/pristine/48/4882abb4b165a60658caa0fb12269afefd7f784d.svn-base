#include "yjdnq_uart_handler.h"
yjdnq_uart_handler hd;
yjdnq_uart_handler::yjdnq_uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud9600;
}

yjdnq_uart_handler::~yjdnq_uart_handler()
{
    if (serial){
        delete serial;
    }
}
bool yjdnq_uart_handler::can_read_uart()
{
    QByteArray tmp = serial->peek(1);
    u_int8_t *data = (u_int8_t *)tmp.data();
    if (data[0] != 0xFF){
        tmp = serial->peek(1000);
        if (!tmp.contains('\n')){
            return false;
        }else{
            return true;
        }
    }else{
        if (serial->bytesAvailable() < 5){
            return false;
        }else{
            tmp = serial->peek(5);
            data = (u_int8_t *)tmp.data();
            uart_cmd_len = data[4] + 5 + 1;
            if (serial->bytesAvailable() < uart_cmd_len){
                return false;
            }else{
                return true;
            }
        }
    }
}

bool yjdnq_uart_handler::data_is_cmd()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    QString s = QTime::currentTime().toString();
    if (data[0] == 0xFF && data[1] == 0xAA && udata.length() == 6 + data[4]){
        qDebug("[%s]get uart cmd:%u", s.toLatin1().data(), data[2]);
        return true;
    }else{
        return false;
    }
}
bool yjdnq_uart_handler::check_sum()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    u_int8_t c = yjdnq_checksum_calc(data, 5 + data[4]);
    u_int8_t b =  data[5 + data[4]];
#ifdef DEBUG_LOG
    qDebug("%s", udata.toHex().data());
    qDebug("%u---%u", c, b);
#endif
    if (c != b){
        qDebug("check sum error!");
        return false;
    }else{
        return true;
    }
}

void yjdnq_uart_handler::init_serial_param()
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
u_int8_t *yjdnq_uart_handler::generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len)
{
    u_int8_t *cmd_buf = NULL;
    u_int8_t total_len = 5 + *len + 5 + 1;//头部5字节，pad5字节，校验和1字节
    cmd_buf = (u_int8_t *)calloc(total_len, 1);

    cmd_buf[0] = 0xFF;
    cmd_buf[1] = 0xAA;
    cmd_buf[2] = cmd;
    cmd_buf[3] = 1;//设备类型
    cmd_buf[4] = *len + 5;
    if (param){
        memcpy(&cmd_buf[5], param, *len);
    }
    cmd_buf[total_len - 1] = yjdnq_checksum_calc(cmd_buf, total_len - 1);
    *len = total_len;
    return cmd_buf;
}
u_int8_t yjdnq_uart_handler::yjdnq_checksum_calc(u_int8_t *buf, u_int8_t len)
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
u_int8_t yjdnq_uart_handler::get_cmd_from_pkt()
{
    return udata.data()[2];
}
void yjdnq_uart_handler::init_self_widgets(int type, widgets_t *w)
{
    switch (type){
        case PRODUCT_TEST_BOTTON:
            w->self_widget = new QPushButton(w->swi->widget_name);
            connect((QPushButton *)w->self_widget, &QPushButton::clicked, &hd, &yjdnq_uart_handler::product_test_send);
            break;
        default:
            break;
    }
}
void yjdnq_uart_handler::product_test_send()
{
    u_int8_t len = 0;
    u_int8_t *pkt = generate_uart_reply_pkt(PRODUCT_TEST_CMD, 0, &len);
    uart_send(pkt, len);
    free(pkt);
}

void yjdnq_uart_handler::update_arg(update_arg_type type)
{
    uart_stat_arg *ua = (uart_stat_arg *)arg;
    switch(type){
        case ARG_TO_UI:
            ua->timer = ua->house_temp;
            ua->house_temp = ua->back_house_temp;
            break;
        case UI_TO_ARG:
            ua->back_house_temp = ua->house_temp;
            break;
        default:
            break;
    }
}
