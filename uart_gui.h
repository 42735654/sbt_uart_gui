#ifndef UART_GUI_H
#define UART_GUI_H
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QMainWindow>
#include "uart_handler.h"
#include <QGridLayout>
#include <QWidget>
#include <QStackedLayout>
#include <QFormLayout>
#include <QDebug>
#define MAX_COL     24
#define MAX_ROWS    16
#define TEXT_BROWSER_COL        (MAX_COL * 2 / 3)
#define WIDGETS_COL     (MAX_COL - TEXT_BROWSER_COL)

class uart_gui:public QMainWindow
{
    Q_OBJECT

    QComboBox *port_index;
    QWidget *for_cert;
    QLabel *uart_stat;
    QLineEdit *send_text;
    QPushButton *send;
    QTextBrowser *uart_log;
    QGridLayout *main_lay;
    QFormLayout *widgets_layout;
    int rows;
    int role;
    int count;
public:
    uart_gui(uart_handler *hd);
    void init_widgets();
    void init_layout();
    void add_widgets(QWidget *any_widgets, bool full_row = false);
    void init_serial_port();

    void init_connections();
    virtual ~uart_gui() = 0;
    virtual void set_ui_by_arg(){}
    virtual void set_arg_by_ui(){}
    virtual void set_arg_by_uart(){}
    virtual void ui_refresh(){}
    virtual void uart_data_handle(){}
    virtual void send_uart_data(){}

public slots:
    void on_port_index_currentIndexChanged(const QString &arg1);
protected:
    uart_handler *uhd;
};

#endif // UART_GUI_H
