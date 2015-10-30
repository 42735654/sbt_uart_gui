#include "qp_break.h"
#include <string.h>

//千帕锅串口数据开始标志
#define QP_BREAK_UART_BEGIN_FLAG                 0x02
//千帕锅串口数据中的addr
#define QP_BREAK_UART_ADDR                           0x1e
//cmd对应的索引号
#define QP_BREAK_UART_CMD_INDEX                 0x03

#define QP_BREAK_UART_SEND_COMMON_LEN             5
qp_break::qp_break()
{
    memset(&qs, 0, sizeof(qp_break_status));
    keep_temp = 0;
    keep_time = 0;
    powerOn = 0;
    wifi_stat = 0;
}
qp_break::~qp_break()
{

}
u_int8_t qp_break::calc_crc(u_int8_t *buf, u_int8_t len)
{
    u_int8_t i, sum = 0;
    u_int8_t crc = 0;

    for (i = 0; i < len; i++) {
        sum += buf[i];
    }
    crc = ((~sum) & 0xff) + 1;
    return crc;
}

u_int8_t *qp_break::qp_break_uart_pkt_generate(u_int8_t cmd, u_int8_t *data, u_int8_t len)
{
    u_int8_t *re_buf = NULL;
    u_int8_t total_len = 0;
    qp_break_cmd_head_t *send_cmd;
    u_int8_t *ptr = NULL;
    if (NULL == data && len != 0){
        return NULL;
    }
    total_len = QP_BREAK_UART_SEND_COMMON_LEN + len;
    re_buf = new u_int8_t[total_len];
    if (!re_buf){
        return NULL;
    }
    send_cmd = (qp_break_cmd_head_t *)re_buf;
    send_cmd->sync = QP_BREAK_UART_BEGIN_FLAG;
    send_cmd->cmd = cmd;
    send_cmd->addr = QP_BREAK_UART_ADDR;

    send_cmd->data_len = total_len;
    ptr = (u_int8_t *)send_cmd;

//    for (i = 0; i < sizeof(qp_break_cmd_head_t); i++) {
//        sum += ptr[i];
//    }
//    for (i = 0; i < len; i++) {
//        sum += data[i];
//    }
//    crc = ((~sum) & 0xff) + 1;
    ptr = (u_int8_t *)(send_cmd + 1);
    memcpy(ptr, data, len);
    ptr[len] = (u_int8_t)calc_crc(re_buf, total_len - 1);
    return re_buf;
}
bool qp_break::data_is_qp_break_cmd(char *pkt, u_int8_t len)
{
    qp_break_cmd_head_t *uart_head = (qp_break_cmd_head_t *)pkt;
//    qDebug("addr=%u,sync=%u,uart_len=%u, rcv_len=%u\n",
//           uart_head->addr, uart_head->sync, uart_head->data_len, len);
    if (uart_head->addr != QP_BREAK_UART_ADDR ||
            uart_head->sync != QP_BREAK_UART_BEGIN_FLAG ||
            uart_head->data_len > len){
        return false;
    }else{
        return true;
    }
}

