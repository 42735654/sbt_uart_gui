#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qp_break.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_5->setDisabled(true);
    ui->lineEdit_6->setDisabled(true);
    ui->comboBox_6->setDisabled(true);
    initSeialPort();
}

MainWindow::~MainWindow()
{
    if (serial){
        serial->close();
        delete serial;
    }
    delete ui;
}

void MainWindow::initSeialPort()
{
    serial = NULL;
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        ui->comboBox->addItem("没有可用串口");
        return;
    }
    foreach (QSerialPortInfo info, infos) {
        ui->comboBox->addItem(info.portName());
    }
}
void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(serial){
        serial->close();
        delete serial;
    }

    serial = new QSerialPort(arg1);
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));

    bool x = serial->open(QIODevice::ReadWrite);      //读写打开
    if (x == true){
        ui->label->setText("[开启成功！]");
    }else{
        ui->label->setText("[开启失败！]");
        return;
    }
    serial->setBaudRate(QSerialPort::Baud115200);  //波特率
    serial->setDataBits(QSerialPort::Data8); //数据位
    serial->setParity(QSerialPort::NoParity);    //无奇偶校验
    serial->setStopBits(QSerialPort::OneStop);   //停止位1
    serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
}
void MainWindow::serialRead()
{
    while (serial->bytesAvailable()){
        data = serial->readLine();
        if (q.data_is_qp_break_cmd(data.data(), data.length())){
            memcpy(&last_q, &q, sizeof(q));
            unsigned char cmd = data.data()[3];
            qDebug("Get uart Cmd:[%x]!\n", cmd);
            set_qs_by_uart_cmd(cmd, (u_int8_t *)data.data());
            if (memcmp(&last_q, &q, sizeof(q)))
                set_ui_by_qs();
            qs_send_to_uart();
        }else{
            ui->textBrowser->append(data);
        }
    }
}
void MainWindow::set_qs_idle()
{
    q.qs.status = QP_BREAK_IDLE;
    q.qs.id = 0;
    q.qs.mix_power = 0;
    q.qs.error = 0;
    q.keep_temp = 0;
    q.keep_time = 0;
    q.qs.temp = 0;
}

void MainWindow::set_qs_by_uart_cmd(unsigned char cmd, u_int8_t *uart_data)
{
    u_int8_t power = uart_data[4];
    switch (cmd){
        //开关机命令
        case QP_BREAK_UART_CMD_POWER_ON_OFF:
            qDebug("data[4] is %u\n", uart_data[4]);
            if (power){
                BIT_SET(q.qs.mode, 0);
                q.powerOn = true;
            }else{
                BIT_CLEAN(q.qs.mode, 0);
                set_qs_idle();
                q.powerOn = false;
            }
            break;

        //执行IC方案
        case QP_BREAK_UART_CMD_IC_PLAN_EXEC:
            qDebug("data[4] is %u\n", uart_data[4]);
            if (q.powerOn == false){
                qDebug("q.powerOn == false\n");
                break;
            }
            q.qs.id = uart_data[4];
            if (q.qs.id == 0){
                set_qs_idle();
            }else{
                q.qs.status = QP_BREAK_MAKING;
                q.qs.mix_power = 5;
                q.keep_temp = 70;
                q.keep_time = 30;
                q.qs.temp = 80;
            }
            break;

            //执行自定义方案
        case QP_BREAK_UART_CMD_APP_PLAN_EXEC:
            if (q.powerOn == false){
                break;
            }
            qDebug("QP_BREAK_UART_CMD_APP_PLAN_EXEC\n");
            for (int i = 0; i < uart_data[1]; i++){
                qDebug("%d", uart_data[i]);
            }
            qDebug("dump QP_BREAK_UART_CMD_APP_PLAN_EXEC data end!\n");
            q.qs.id = uart_data[4];
            q.qs.status = QP_BREAK_MAKING;
            q.keep_temp = uart_data[32];
            q.keep_time = uart_data[33];
            break;
            //停止自定义方案
        case QP_BREAK_UART_CMD_APP_PLAN_STOP:
            if (q.powerOn == false){
                break;
            }
            set_qs_idle();
            break;
            //查询
        case QP_BREAK_UART_CMD_QUERY:
            break;

        case QP_BREAK_UART_CMD_ASK_RESET:
            q.qs.error = 0;
            break;
        case QP_BREAK_UART_CMD_WIFI_STAT:
            q.wifi_stat = uart_data[4];
            break;
         case QP_BREAK_UART_CMD_TEST_REPLY:
            q.wifi_stat = 5;
            break;
        default:
            break;
    }
}

void MainWindow::set_ui_by_qs()
{
    qDebug("set_ui_by_qs:id is %u\n", q.qs.id);
    ui->lineEdit_2->setText(QString::number((long)q.qs.id));
    ui->comboBox_4->setCurrentIndex(q.qs.status);
    ui->comboBox_3->setCurrentIndex(q.qs.mix_power);
    int mode_index = 0;
    if (!BIT_IS_SET(q.qs.mode, 0)){
        mode_index = 0;
        ui->comboBox_5->setCurrentIndex(0);
    }else{
        if (BIT_IS_SET(q.qs.mode, 1)){
         mode_index = 1;
        }
        if (BIT_IS_SET(q.qs.mode, 2)){
         mode_index = 2;
        }
        ui->comboBox_5->setCurrentIndex(1);
    }
    ui->comboBox_2->setCurrentIndex(mode_index);
    ui->lineEdit_5->setText(QString::number(q.keep_temp));
    ui->lineEdit_6->setText(QString::number(q.keep_time));
    ui->lineEdit_4->setText(QString::number(q.qs.error));
    ui->lineEdit_3->setText(QString::number(q.qs.temp));
    ui->comboBox_6->setCurrentIndex(q.wifi_stat);
}

void MainWindow::set_qs_by_ui()
{
    q.qs.id = ui->lineEdit_2->text().toInt();
    q.qs.status = ui->comboBox_4->currentIndex();
    q.qs.mix_power = ui->comboBox_3->currentIndex();
    int mode_index = ui->comboBox_2->currentIndex();
    BIT_CLEAN(q.qs.mode, 1);
    BIT_CLEAN(q.qs.mode, 2);
    if (mode_index == 1){
        BIT_SET(q.qs.mode, 1);
    }
    if (mode_index == 2){
        BIT_SET(q.qs.mode, 2);
    }
    if (ui->comboBox_5->currentIndex() == 0){
        q.qs.mode = 0;
    }else{
        BIT_SET(q.qs.mode, 0);
        q.powerOn = true;
    }
    q.qs.error = ui->lineEdit_4->text().toInt();
    q.qs.temp = ui->lineEdit_3->text().toInt();
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
}
void MainWindow::qs_send_to_uart()
{
    set_qs_by_ui();
    u_int8_t *pkt = q.qp_break_uart_pkt_generate(QP_BREAK_UART_CMD_REPLY, (u_int8_t *)&q.qs, sizeof(qp_break_status));
    serial->write((char *)pkt, pkt[1]);
    delete pkt;
}
void MainWindow::ask_wifi_reset()
{
    u_int8_t tmp = 0;
    u_int8_t *pkt = q.qp_break_uart_pkt_generate(QP_BREAK_UART_CMD_CON_WIFI, &tmp, sizeof(u_int8_t));
    serial->write((char *)pkt, pkt[1]);
    delete pkt;
}
void MainWindow::ask_wifi_test()
{
    u_int8_t tmp = 0;
    u_int8_t *pkt = q.qp_break_uart_pkt_generate(QP_BREAK_UART_CMD_TEST, &tmp, sizeof(u_int8_t));
    serial->write((char *)pkt, pkt[1]);
    delete pkt;
}

void MainWindow::on_pushButton_clicked()
{
    if (!serial){
        return;
    }
    serial->write(ui->lineEdit->text().toStdString().c_str());
    ui->lineEdit->clear();
    return;

}

void MainWindow::on_pushButton_2_clicked()
{
    if (!serial){
        return;
    }
    set_qs_by_ui();
    qs_send_to_uart();
}

void MainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButton_4_clicked()
{
    ask_wifi_reset();
    memset(&q, 0, sizeof(q));
    set_ui_by_qs();
}

void MainWindow::on_pushButton_5_clicked()
{
    ask_wifi_test();
}
