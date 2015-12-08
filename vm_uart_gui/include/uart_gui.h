#ifndef UART_GUI_H
#define UART_GUI_H
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QStackedLayout>
#include <QFormLayout>
#include <QDebug>
#include <QByteArray>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QCheckBox>
#include <QSignalMapper>

#include "type_def.h"
#include "config.h"
#include "uart_handler.h"
class uart_gui;
extern uart_gui *p;
#define INFO(...)       p->__log_to_ui(QString("").sprintf(__VA_ARGS__), LOG_TYPE_INFO)
#define WARN(...)       p->__log_to_ui(QString("").sprintf(__VA_ARGS__), LOG_TYPE_WARN)
#define ERROR(...)       p->__log_to_ui(QString("").sprintf(__VA_ARGS__), LOG_TYPE_ERROR)
#define DEBUG(...)       p->__log_to_ui(QString("").sprintf(__VA_ARGS__), LOG_TYPE_DEBUG)
#define UART_STAT_BUF_LEN   256 * 4
#define MAX_COL 18
struct uart_config{
    u_int8_t uart_stat_buf[UART_STAT_BUF_LEN];
    u_int8_t last_uart_stat_buf[UART_STAT_BUF_LEN];
};


class uart_gui:public QMainWindow
{
    Q_OBJECT
private:
    QComboBox *port_index;      //串口列表

    QWidget *for_cert;                  //用来布局

    QTextBrowser *uart_log;             //终端显示框

    QLineEdit *send_text;               //发送编辑框

    QCheckBox *send_hex;               //发送16进制
    QCheckBox *if_log_file;             //输出日志到文件

    QPushButton *pb_pause;                              //暂停输出

    QLabel *uart_stat;                    //串口状态

    QGridLayout *main_lay;              //主布局
    QGridLayout *widgets_layout;    //控件布局

    QPalette window_bg;
    QTextStream* log_stream;

    QFile *log_file;

    QMutex file_mutex;
    QMutex tb_mutex;

    QSignalMapper *smp;
    QMenu *menu;

    //自动添加控件的数据信息

    int next_spare_index;
    bool text_browser_pause;
protected:
    //自定义布局列表
    widgets_list sw;

    uart_config cfg;
    uart_handler *uhd;                         //串口操作类

protected:
    void adjust_windows();
    void add_widget_by_info(status_widgets_info *swi);                      //根据描述添加控件
    void set_ui_by_arg(u_int8_t *data);                   //根据参数设置控件显示
    void set_arg_by_ui(u_int8_t *data);                   //根据UI控件设置参数
    void del_smp(){
        delete smp;
        smp = NULL;
    }

    typedef void (uart_gui::*CON_CALLBACK)();
    QPushButton *add_pb(const char *name, CON_CALLBACK pcb);
    void menu_add_action(const char *name, CON_CALLBACK cb);

private:
    void stop_log_file();
    void set_pb_background_pic(QPushButton *pb);
    void set_lineedit_background_pic(QLineEdit *pb);
    void set_text_style(QWidget *pb);
    void init_log_file(QString filename);
    void init_base_widgets();       //初始化基础控件
    void init_layout();                     //初始化布局
    QPoint get_new_point(bool fullrow);
protected:
    uart_gui(uart_handler *hd);
    virtual ~uart_gui();
    void del_widgets(QWidget *any_widgets, bool full_row = false);
    void add_widgets(QWidget *any_widgets, bool full_row = false);  //向控件布局中添加一个控件
    void add_widgets_by_infos(status_widgets_info *swi, int count);     //根据描述列表添加控件
    void del_self_widgets();
public slots:
    virtual void on_port_index_currentIndexChanged(const QString &arg1);        //串口选择改变时
    void send_line_text();                                                                                  //发送按钮
    void text_browser_text_changed();                                                           //设置textbrower显示始终在最后一行
    void __log_to_ui(QString log, int type = LOG_TYPE_INFO);                                                                  //打印信息到textbrowser中
    void __log_to_file(QString log);                   //打印信息到文件中
    void clear_browser(){
        uart_log->clear();
        uart_log->setFontPointSize(12);
    }
    virtual void on_set_pushed() = 0;
    void pause_browser();
    void on_checkbox_log_to_file();
    virtual void handle_uart_data() = 0;
    virtual void pushbotton_callback(int id) = 0;
};

#endif // UART_GUI_H
