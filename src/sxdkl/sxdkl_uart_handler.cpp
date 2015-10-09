#include "sxdkl_uart_handler.h"
sxdkl_uart_handler hd;
sxdkl_uart_handler::sxdkl_uart_handler()
{
    serial = NULL;
    btl = QSerialPort::Baud9600;
}

sxdkl_uart_handler::~sxdkl_uart_handler()
{
    if (serial){
        delete serial;
    }
}
bool sxdkl_uart_handler::can_read_uart()
{
    if (serial->canReadLine()){
        return true;
    }else{
        return false;
    }
}

bool sxdkl_uart_handler::data_is_cmd()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    QString s = QTime::currentTime().toString();
    if (data[0] == 0xAA ){
        qDebug("[%s]get uart cmd:%u", s.toLatin1().data(), get_cmd_from_pkt());
        return true;
    }else{
        return false;
    }
}
bool sxdkl_uart_handler::check_sum()
{
    u_int8_t *data = (u_int8_t *)udata.data();
    u_int8_t c = sxdkl_checksum_calc(&data[1], 10);
    u_int8_t b =  data[11];
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

void sxdkl_uart_handler::init_serial_param()
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
u_int8_t *sxdkl_uart_handler::generate_uart_reply_pkt(u_int8_t cmd, u_int8_t *param, u_int8_t *len)
{
        u_int8_t *cmd_buf = NULL;
        u_int8_t total_len = 13;
        uart_stat_arg *ua = (uart_stat_arg *)arg;
        ua->cmd_ack.recv_suc = 1;
        cmd_buf = (u_int8_t *)calloc(total_len, 1);
        memcpy(cmd_buf, param, *len);

        cmd_buf[0] = 0xaa;
        cmd_buf[12] = sxdkl_checksum_calc(&cmd_buf[1], total_len - 2);
        *len = total_len;
        return cmd_buf;
}
u_int8_t sxdkl_uart_handler::sxdkl_checksum_calc(u_int8_t *buf, u_int8_t len)
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
u_int8_t sxdkl_uart_handler::get_cmd_from_pkt()
{
    return udata.data()[1];
}
void sxdkl_uart_handler::init_self_widgets(int type, widgets_t *w)
{
    switch (type){
        case PRODUCT_TEST_BOTTON:
            w->self_widget = new QPushButton(w->swi->widget_name);
            connect((QPushButton *)w->self_widget, &QPushButton::clicked, &hd, &sxdkl_uart_handler::product_test_send);
            break;
    case SET_WIFI_TO_AP:
        w->self_widget = new QPushButton(w->swi->widget_name);
        connect((QPushButton *)w->self_widget, &QPushButton::clicked, &hd, &sxdkl_uart_handler::set_wifi_to_ap);
        break;
    case WIFI_RESET:
        w->self_widget = new QPushButton(w->swi->widget_name);
        connect((QPushButton *)w->self_widget, &QPushButton::clicked, &hd, &sxdkl_uart_handler::wifi_reset);
        break;
        default:
            break;
    }
}
void sxdkl_uart_handler::product_test_send()
{
    uart_stat_arg *ua = (uart_stat_arg *)arg;
    ua->cmd_ack.wifi_test = 1;
    uart_cmd_reply_query(0);
     ua->cmd_ack.wifi_test = 0;
}
void sxdkl_uart_handler::set_wifi_to_ap()
{
    uart_stat_arg *ua = (uart_stat_arg *)arg;
    ua->cmd_ack.wifi_disbind = 1;
    uart_cmd_reply_query(0);
     ua->cmd_ack.wifi_disbind = 0;
}
void sxdkl_uart_handler::wifi_reset()
{
    uart_stat_arg *ua = (uart_stat_arg *)arg;
    ua->cmd_ack.reset = 1;
    uart_cmd_reply_query(0);
     ua->cmd_ack.reset = 0;
}

void sxdkl_uart_handler::update_arg(update_arg_type type)
{
    uart_stat_arg *ua = (uart_stat_arg *)arg;
    switch(type){
        case ARG_TO_UI:
            ua->anion = ua->fb.anion;
            ua->timer_on = ua->tb.on;
            ua->timer_h = ua->tb.hour;
            ua->fire_level = ua->lb.fire_level;
            ua->simulat_wood = ua->lb.simulat_wood;
            ua->power = ua->fb.power;
            ua->temp_unit = ua->fb.temp_unit;
            ua->router_suc = ua->wb.router_stat;
            ua->server_suc = ua->wb.net_stat;
            break;
    case UI_TO_ARG:
        ua->fb.anion = ua->anion>0?1:0;
        ua->fb.power = ua->power>0?1:0;
        ua->fb.temp_unit = ua->temp_unit>0?1:0;
        ua->lb.fire_level = ua->fire_level>0?1:0;
        ua->lb.simulat_wood = ua->simulat_wood>0?1:0;
        ua->tb.on = ua->timer_on>0?1:0;
        ua->tb.hour = ua->timer_h>0?1:0;

        ua->wb.router_stat = ua->router_suc > 0?1:0;
        ua->wb.net_stat = ua->server_suc > 0?1:0;
        break;
    default:
        break;
    }
}
