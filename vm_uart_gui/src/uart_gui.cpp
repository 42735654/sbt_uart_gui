#include "config.h"
#include "type_def.h"
#include "uart_gui.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDir>
#include <QMetaMethod>
#include<Winsock2.h>
//根据内存buf设置UI显示
void uart_gui::set_ui_by_arg(u_int8_t *data)
{
    int n;
    u_int8_t index;
    int *value = (int *)data;
    for (int i = 0; i < sw.swi_count; i++){
        if (sw.swis[i].swi->widget_type != WTYPE_LABEL_TEXTLINE){
            continue;
        }
        index = (sw.swis[i].swi)->index_in_arg;
        n = ntohl(value[index]);
        ((QLineEdit *)sw.swis[i].lineedit)->setText(QString::number(n));
    }
}
//根据UI界面值设置内存buf
void uart_gui::set_arg_by_ui(u_int8_t *data)
{
    int n;
    u_int8_t index;
    int *value = (int *)data;
    for (int i = 0; i < sw.swi_count; i++){
        if (sw.swis[i].swi->widget_type != WTYPE_LABEL_TEXTLINE){
            continue;
        }
        index = (sw.swis[i].swi)->index_in_arg;
        n = ((QLineEdit *)sw.swis[i].lineedit)->text().toInt();
        value[index] = htonl(n);
        cfg.last_uart_stat_buf[index] = n;
    }
}
//初始化日志文件
void uart_gui::init_log_file(QString filename)
{
    delete log_stream;
    delete log_file;
    log_stream = new QTextStream;
    log_file = new QFile(filename, this);
    if (log_file->open(QIODevice::Append))
    {
        log_stream->setDevice(log_file);
    }
}
void uart_gui::stop_log_file()
{
    if (log_file->isOpen()){
        log_file->close();
    }
    delete log_stream;
    log_stream = NULL;

    delete log_file;
    log_file = NULL;

}
void uart_gui::__log_to_file(QString log){
    if (!log_file->isOpen()){
        init_log_file(QDir::currentPath() + "/uart_gui.log");
    }
    //局部变量，函数结束自动释放锁
    QMutexLocker locker(&file_mutex);
    *log_stream << log;
    *log_stream << "\n";
    log_stream->flush();
}

void uart_gui::del_self_widgets()
{
    int n = sw.swi_count;
    for (int i = 0; i < n ; i++){
        if (sw.swis[i].swi->widget_type == WTYPE_PUSHBOTTON){
            del_widgets(sw.swis[i].self_widget);
        }else if (sw.swis[i].swi->widget_type == WTYPE_LABEL_TEXTLINE){
            del_widgets(sw.swis[i].label);
            del_widgets(sw.swis[i].lineedit);
            if (sw.swis[i].self_widget){
                del_widgets(sw.swis[i].self_widget);
            }
        }
        delete sw.swis[i].swi;
        sw.swi_count --;
    }
    del_smp();
    memset(&sw, 0, sizeof(sw));
}
QPushButton *uart_gui::add_pb(const char *name, CON_CALLBACK pcb)
{
    QPushButton *pb = new QPushButton(name, this);
    connect(pb, &QPushButton::clicked, this, pcb);
    set_pb_background_pic(pb);
    add_widgets(pb);
    return pb;
}
uart_gui::uart_gui(uart_handler *hd)
{
    //置为NULL
    log_stream = NULL;
    log_file = NULL;
    smp = NULL;
    //设置串口操作类
    uhd = hd;
    //UI布局相关

    sw.swi_count = 0;

    text_browser_pause = false;
    memset(&cfg, 0, sizeof(cfg));
    memset(&sw, 0, sizeof(sw));
    //初始化框体布局
    init_layout();
    //初始化基础控件
    init_base_widgets();

    this->resize(1050, 600);

    this->setAutoFillBackground(true);
    window_bg.setBrush(QPalette::Background, QBrush(QPixmap(":/wbg.png")));
    this->setPalette(window_bg);
}
//根据描述结构体添加控件
void uart_gui::add_widget_by_info(status_widgets_info *swi)
{
    switch(swi->widget_type){
    case WTYPE_PUSHBOTTON:
            if (smp == NULL){
                smp = new QSignalMapper(this);
                connect(smp, SIGNAL(mapped(int)), this, SLOT(pushbotton_callback(int)));
            }
            sw.swis[sw.swi_count].self_widget = new QPushButton(swi->widget_name,this);
            smp->setMapping(sw.swis[sw.swi_count].self_widget, sw.swis[sw.swi_count].swi->index_in_arg);
            connect((QPushButton *)sw.swis[sw.swi_count].self_widget, SIGNAL(clicked()), smp, SLOT(map()));
            //设置按钮颜色
            set_pb_background_pic((QPushButton *)sw.swis[sw.swi_count].self_widget);
            add_widgets(sw.swis[sw.swi_count].self_widget);
            break;
    case WTYPE_CHECKBOX:
            break;

    case WTYPE_LABEL_TEXTLINE:
            //WTYPE_LABEL_TEXTLINE需从第一列开始，否则需占位
            if (next_spare_index % 2){
                sw.swis[sw.swi_count].self_widget = new QLabel(this);
                add_widgets(sw.swis[sw.swi_count].self_widget);
            }
            sw.swis[sw.swi_count].label = new QLabel(swi->widget_name, this);
            set_text_style(sw.swis[sw.swi_count].label);
            sw.swis[sw.swi_count].lineedit = new QLineEdit(this);
            set_lineedit_background_pic((QLineEdit *)sw.swis[sw.swi_count].lineedit);

            add_widgets(sw.swis[sw.swi_count].label);
            add_widgets(sw.swis[sw.swi_count].lineedit);
                break;
    default:
            break;
    }
    sw.swi_count++;

}
//初始化用户自定义控件
void uart_gui::add_widgets_by_infos(status_widgets_info *swi, int count)
{
    for (int i = 0; i < count; i++){
        sw.swis[sw.swi_count].swi = new status_widgets_info;
        memcpy(sw.swis[sw.swi_count].swi, &swi[i], sizeof(status_widgets_info));
        add_widget_by_info(sw.swis[sw.swi_count].swi);
    }
    //INFO(" sw.swi_count=%d",  sw.swi_count);
}
void uart_gui::menu_add_action(const char *name, CON_CALLBACK cb)
{
    QAction *act = new QAction(name, this);
    connect(act, &QAction::triggered, this, cb);
    menu->addAction(act);
}

void uart_gui::pause_browser()
{
    text_browser_pause = !text_browser_pause;
    if (text_browser_pause){
        uart_log->append("---------------------------------------------------------");
        uart_log->append("暂停输出串口数据...");
        pb_pause->setText("继续输出");
    }else{
        uart_log->append("继续输出串口数据...");
        uart_log->append("---------------------------------------------------------");
        pb_pause->setText("暂停输出");
    }
}

void uart_gui::on_checkbox_log_to_file()
{
    if (if_log_file->isChecked()){
        init_log_file(  QDir::currentPath() + "/uart_gui.log");
        INFO("调试文件:%s", log_file->fileName().toLatin1().data());
    }else{
        INFO("停止输出到文件");
        stop_log_file();
    }
}
void uart_gui::text_browser_text_changed()
{
    uart_log->moveCursor(QTextCursor::End);
}

void uart_gui::__log_to_ui(QString s, int type)
{
    QMutexLocker locker(&tb_mutex);
    QString __log;
    switch(type){
        case LOG_TYPE_INFO:
            uart_log->setTextColor(QColor(200, 200, 200));
            uart_log->setFontPointSize(13);
            __log = "[INFO]";
            break;
        case LOG_TYPE_WARN:
            uart_log->setTextColor(QColor(72, 211, 143));
            uart_log->setFontPointSize(13);
            __log = "[WARN]";
            break;
        case LOG_TYPE_ERROR:
            uart_log->setTextColor(QColor(159, 228, 24));
            uart_log->setFontPointSize(16);
            __log = "[ERROR]";
            break;
        case LOG_TYPE_DEBUG:
            uart_log->setTextColor(QColor(200, 100, 50));
            uart_log->setFontPointSize(13);
            __log = "[DEBUG]";
            break;
        default:
            break;
    }
     __log.append("[" +QTime::currentTime().toString() + "." +
                  QString::number(QTime::currentTime().msec()) + "]"+ s);
    if (!text_browser_pause){
        uart_log->append(__log);
    }
    if (if_log_file->isChecked()){
        __log_to_file(__log);
    }
}
void uart_gui::send_line_text()
{
    QString to_send = send_text->text();

    if (send_hex->isChecked()){
        int  count = 0;
        u_int8_t *hex_buf = new u_int8_t[to_send.length() / 2 + 1];
        int i = 0;
        while(count < to_send.length() - 1){
            QByteArray h;
            h.append(to_send.at(count).toLatin1());
            h.append(to_send.at(count + 1).toLatin1());
            char x = (char)strtol(h.data(), NULL, 16);
            hex_buf[i++] = x;
            count += 2;
        }
        uhd->uart_send(hex_buf, i);
        delete hex_buf;
    }else{
        uhd->uart_send((u_int8_t *)to_send.toLatin1().data(), to_send.length());
    }
}
void uart_gui::set_pb_background_pic(QPushButton *pb)
{
    pb->setAutoFillBackground(true);
    pb->setFlat(true);
    pb->setStyleSheet("background-image:url(:/pb_bg.png);color:rgb(160, 160, 160);font: bold  \"Times New Roman\";");
}
void uart_gui::set_lineedit_background_pic(QLineEdit *pb)
{
    pb->setStyleSheet("QLineEdit{background:url(\":/pb_bg.png\");font: bold  large \"Times New Roman\"; \
                                                                            font-size:14px;color:rgb(160, 160, 160);border-radius:8px; \
                                                                                border:1px solid rgb(160, 160, 160)};");
}
void uart_gui::set_text_style(QWidget *pb)
{
    pb->setStyleSheet("font: bold  large \"Times New Roman\"; \
                                   color:rgb(160, 160, 160);border-radius:8px; ");
}
void uart_gui::init_base_widgets()
{
    port_index = new QComboBox((this));
    port_index->setStyleSheet("QComboBox {background-color:transparent;color:rgb(160, 160, 160); \
                                                     font: bold  large \"Times New Roman\";}");

    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        port_index->addItem("没有可用串口!");
        __log_to_ui("没有可用串口!");
        return;
    }
    port_index->addItem("串口选择");
    foreach (QSerialPortInfo info, infos) {
        port_index->addItem(info.portName());
    }
    connect(port_index, SIGNAL(currentIndexChanged(QString)), this,
                            SLOT(on_port_index_currentIndexChanged(QString)));
    add_widgets(port_index);

    uart_stat = new QLabel(this);
    uart_stat->setText("未打开串口");
    set_text_style(uart_stat);
    add_widgets(uart_stat);

    send_text = new QLineEdit(this);
    set_lineedit_background_pic(send_text);
    add_widgets(send_text, true);

    send_hex = new QCheckBox("发送16进制", this);
    set_text_style(send_hex);
    add_widgets(send_hex);

    if_log_file = new QCheckBox("输出到文件", this);
    set_text_style(if_log_file);
    //if_log_file->setChecked(true);
    connect(if_log_file, SIGNAL(clicked(bool)), this, SLOT(on_checkbox_log_to_file()));
    add_widgets(if_log_file);

    add_pb("发送", &send_line_text);
    add_pb("设置到设备", &on_set_pushed);
    add_pb("清屏", &clear_browser);
    pb_pause = add_pb("暂停输出", &pause_browser);

    uart_log = new QTextBrowser(this);
    QPalette palette=uart_log->palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
    uart_log->setPalette(palette);
    connect(uart_log, SIGNAL(textChanged()), this, SLOT(text_browser_text_changed()));
    main_lay->addWidget(uart_log, 0, 1, 1, 1);

    menu =menuBar()->addMenu(tr("&功能"));
}
void uart_gui::on_port_index_currentIndexChanged(const QString &port)
{
    uhd->close_serial();
    if (port_index->currentIndex() == 0){
        return;
    }
    bool x = uhd->open_serial_port(port);
    if (x == true){
        uart_stat->setText("[串口开启成功]");
        INFO("%s开启成功", port.toLatin1().data());
    }else{
        uart_stat->setText("[串口开启失败]");
        INFO("%s开启失败", port.toLatin1().data());
        uhd->serial = NULL;
       port_index->setCurrentIndex(0);
        return;
    }
    uhd->init_serial_param();
    connect(uhd->serial, SIGNAL(readyRead()), this, SLOT(handle_uart_data()));
}
QPoint uart_gui::get_new_point(bool fullrow)
{
    QPoint newpoint(0, 0);

    if (fullrow && next_spare_index % 2){
        next_spare_index ++;
    }
    newpoint = QPoint(next_spare_index % 2 + (next_spare_index / (MAX_COL * 2)) * 2,
                                       next_spare_index / 2 - (next_spare_index / (MAX_COL * 2)) * MAX_COL);
    if (fullrow){
        next_spare_index += 2;
    }else{
        next_spare_index ++;
    }
    return newpoint;
}

//向布局中添加一个控件
void uart_gui::add_widgets(QWidget *any_widgets, bool full_row)
{
    QPoint pos = get_new_point(full_row);
    if (full_row == true){
        widgets_layout->addWidget(any_widgets, pos.y(), pos.x(), 1, 2);
    }else{
        widgets_layout->addWidget(any_widgets, pos.y(), pos.x(), 1, 1);
    }
    adjust_windows();
    //qDebug("add widget to pos(%d,%d)", pos.x(), pos.y());
}
void uart_gui::adjust_windows()
{
    //动态调整布局
    this->resize(900 + (next_spare_index / (2 * MAX_COL)) * 200, 600);
    main_lay->setColumnStretch(0, 2 + (next_spare_index / (2 * MAX_COL)) * 2);
}

void uart_gui::del_widgets(QWidget *any_widgets, bool full_row)
{
    delete any_widgets;
    if (full_row){
        next_spare_index -= 2;
    }else{
        next_spare_index --;
    }
    adjust_windows();
}

void uart_gui::init_layout()
{
    for_cert = new QWidget(this);
    main_lay = new QGridLayout;
    widgets_layout = new QGridLayout;

    main_lay->addLayout(widgets_layout, 0, 0, 1, 1);
    for_cert->setLayout(main_lay);
    this->setCentralWidget(for_cert);

    //控件列表与调试信息比例为1：3
    main_lay->setColumnStretch(0, 2);
    main_lay->setColumnStretch(1, 8);
}

uart_gui::~uart_gui()
{
    if (log_file){
        stop_log_file();
    }
    delete widgets_layout;
    delete main_lay;

    for (int i = 0; i < sw.swi_count ; i++){
        delete sw.swis[i].swi;
    }
}

