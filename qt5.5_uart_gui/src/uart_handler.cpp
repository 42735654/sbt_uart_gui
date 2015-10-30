#include "config.h"
#include "type_def.h"
#include INCLUDE_FILE(INCLUDE_FILE_PART(DST_PROJECT, _cmd_define.h))
#include "uart_handler.h"
uart_handler::uart_handler()
{
    btl = QSerialPort::Baud115200;
    memset(arg, 0, sizeof(arg));
    memcpy(last_arg, arg, sizeof(arg));
    cmds = &cmd_infos[0];
    cmd_count = sizeof(cmd_infos) / sizeof(cmd_info);
}

uart_handler::~uart_handler()
{

}
void uart_handler::log_to_ui(QString s, int type)
{
    emit s_log_to_ui(s, type);
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
    uart_recvie();
}

int uart_handler::uart_send(u_int8_t *buf, u_int8_t len)
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
    DEBUG("↑:%s", s.toHex().data());
    return has_send_len;
}
void uart_handler::uart_recvie()
{
    if (!can_read_uart()){
        return;
    }
    udata = serial->readLine();
    if (data_is_cmd()){
        if (!check_sum()){
            return;
        }
        INFO("↓:%s", udata.toHex().data());
        set_arg_by_uart(get_cmd_from_pkt());
        if (memcmp(arg, last_arg, sizeof(arg))){
            memcpy(last_arg, arg, sizeof(arg));
            update_arg(ARG_TO_UI);
            emit signal_for_uart_to_ui(ARG_CHANGED);
        }
    }else{
        udata.replace('\n', ' ');
        udata.replace('\r', ' ');
        INFO("↓:%s", udata.data());
    }
}
void uart_handler::set_arg_by_uart(u_int8_t cmd)
{
    u_int8_t *data = (u_int8_t *)udata.data();
    bool found = false;
    for (int i = 0; i < cmd_count; i++){
        if (cmd == cmds[i].cmd){
            memcpy(&arg[cmds[i].arg_index], &data[cmds[i].pkt_index], cmds[i].arg_len);
            uart_cmd_reply_query(cmds[i].reply_type);
            found = true;
            INFO("收到命令:%s", cmds[i].cmd_name);
            break;
        }
    }
    if (!found){
        ERROR("未定义命令:%u", cmd);
    }
}
void uart_handler::uart_cmd_reply_query(int type)
{
    u_int8_t len;
    u_int8_t *pkt;

    for (int i = 0; i < cmd_count; i++){
        if (type == cmds[i].reply_type){
            len = cmds[i].reply_len;
            pkt = generate_uart_reply_pkt(type, &arg[cmds[i].reply_index], &len);
            uart_send(pkt, len);
            free(pkt);
            break;
        }
    }
}
