#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "qp_break.h"
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ask_wifi_test();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QByteArray data;

    qp_break q;
    qp_break last_q;
    void initSeialPort();
private slots:
    void serialRead();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void set_ui_by_qs();
    void set_qs_by_ui();
    void set_qs_by_uart_cmd(unsigned char cmd, u_int8_t *uart_data);
    void qs_send_to_uart();
    void set_qs_idle();
    void on_pushButton_2_clicked();
    void on_textBrowser_textChanged();
    void ask_wifi_reset();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
};

#endif // MAINWINDOW_H
