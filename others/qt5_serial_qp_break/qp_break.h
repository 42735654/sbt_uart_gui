#ifndef QP_BREAK_H
#define QP_BREAK_H

#include <QDebug>
typedef   unsigned char u_int8_t;
#define BIT_IS_SET(a,index)	(a & (1 << index))
#define BIT_SET(a,index)		(a |= 1 << index)
#define BIT_CLEAN(a,index)	(a &= ~(1 << index))
//控制命令
#define QP_BREAK_UART_CMD_POWER_ON_OFF           0x64  	 			//开关机
#define QP_BREAK_UART_CMD_IC_PLAN_EXEC           0x65 			// 执行固定方案，id = 0为取消
#define QP_BREAK_UART_CMD_APP_PLAN_EXEC           0x68                 //设置烹饪流程
#define QP_BREAK_UART_CMD_APP_PLAN_STOP           0x69                //设置烹饪流程
#define QP_BREAK_UART_CMD_QUERY     0x6a               				//查询破壁机状态
#define QP_BREAK_UART_CMD_ASK_RESET     0x6c              			//请求故障恢复
#define QP_BREAK_UART_CMD_WIFI_STAT     0x6d              			//wifi状态下发

#define QP_BREAK_UART_CMD_REPLY     0xE4              				//查询收到的回复
#define QP_BREAK_UART_CMD_CON_WIFI     0xE5              			// //请求恢复出厂

#define QP_BREAK_UART_CMD_TEST     0xE6
#define QP_BREAK_UART_CMD_TEST_REPLY    0xE7
typedef struct {
        u_int8_t sync;
        u_int8_t data_len;
        u_int8_t addr;
        u_int8_t cmd;
}qp_break_cmd_head_t;
typedef struct {
    u_int8_t id;
    u_int8_t mode;
    u_int8_t error;
    u_int8_t mix_power;
    u_int8_t temp;
    u_int8_t pad[4];
    u_int8_t status;
}qp_break_status;
enum{
    QP_BREAK_IDLE,
    QP_BREAK_MAKING,
    QP_BREAK_FINISH
};
class qp_break{
       public:
            qp_break();
            ~qp_break();
            qp_break_status qs;
            u_int8_t keep_temp;
            u_int8_t keep_time;
            u_int8_t wifi_stat;
            bool powerOn;
            u_int8_t calc_crc(u_int8_t *buf, u_int8_t len);
            bool data_is_qp_break_cmd(char *pkt, u_int8_t len);
            u_int8_t *qp_break_uart_pkt_generate(u_int8_t cmd, u_int8_t *data, u_int8_t len);
};

#endif // QPG_H
