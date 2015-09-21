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
#include <QByteArray>
#define MAX_WIDGETS_NUM    50
typedef unsigned char u_int8_t;
typedef struct{
    int index_in_arg;
    const char *widget_name;
}status_widgets_info;

typedef struct{
    QLabel *label;
    QLineEdit *lineedit;
    status_widgets_info *swi;
}widgets_t;

typedef struct{
    widgets_t swis[MAX_WIDGETS_NUM];
    int swi_count;
}widgets_list;

class uart_gui:public QMainWindow
{
    Q_OBJECT
private:
    QComboBox *port_index;
    QWidget *for_cert;
    QLabel *uart_stat;
    QLineEdit *send_text;
    QPushButton *send;
    QPushButton *set_arg;
    QTextBrowser *uart_log;
    QGridLayout *main_lay;
    QLabel space;
    QFormLayout *widgets_layout;
    widgets_list sw;
    int rows;
    int role;
    int count;
public:
    uart_gui(uart_handler *hd);
    void init_widgets();
    void init_layout();
    void add_widgets(QWidget *any_widgets, bool full_row = false);
    void init_serial_port();
    void add_widget_by_info(status_widgets_info *swi);
    void add_widgets_by_infos(status_widgets_info *swi, int count);
    void init_connections();

    ~uart_gui();
    virtual void set_ui_by_arg();
    virtual void set_arg_by_ui();
    virtual void uart_data_handle(){}
    virtual void send_uart_data(){}

public slots:
    void on_port_index_currentIndexChanged(const QString &arg1);
    void send_line_text();
    void text_browser_text_changed();
    void __log_to_ui(QString log);
protected:
    uart_handler *uhd;
};

#endif // UART_GUI_H
