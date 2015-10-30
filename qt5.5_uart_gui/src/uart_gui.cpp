#include "config.h"
#include "type_def.h"
#include "uart_gui.h"
#include INCLUDE_FILE(INCLUDE_FILE_PART(DST_PROJECT, _ui_define.h))
#include <QDir>
//根据内存buf设置UI显示
void uart_gui::set_ui_by_arg()
{
    u_int8_t n, index;
    for (int i = 0; i < sw.swi_count; i++){
        if (sw.swis[i].swi->widget_type != WTYPE_LABEL_TEXTLINE){
            continue;
        }
        index = (sw.swis[i].swi)->index_in_arg;
        n = uhd->arg[index];
        ((QLineEdit *)sw.swis[i].lineedit)->setText(QString::number(n));
    }
}
//根据UI界面值设置内存buf
void uart_gui::set_arg_by_ui()
{
    u_int8_t n, index;
    for (int i = 0; i < sw.swi_count; i++){
        if (sw.swis[i].swi->widget_type != WTYPE_LABEL_TEXTLINE){
            continue;
        }
        index = (sw.swis[i].swi)->index_in_arg;
        n = ((QLineEdit *)sw.swis[i].lineedit)->text().toInt();
        uhd->arg[index] = n;
    }
    memcpy(uhd->last_arg, uhd->arg, sizeof(uhd->arg));
    //处理一些上报报文与buf不一致的情况
    uhd->update_arg(uart_handler::UI_TO_ARG);

    uhd->uart_cmd_reply_query();
}
//初始化日志文件
void uart_gui::init_log_file(QString filename)
{
    delete log_stream;
    delete log_file;
    log_stream = new QTextStream;
    log_file = new QFile(filename);
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
uart_gui::uart_gui(uart_handler *hd)
{
    //置为NULL
    log_stream = NULL;
    log_file = NULL;
    //设置串口操作类
    uhd = hd;
    //UI布局相关
    rows = 0;
    count = 0;
    sw.swi_count = 0;
    text_browser_pause = false;
    role = QFormLayout::LabelRole;
    //初始化框体布局
    init_layout();
    //初始化基础控件
    init_base_widgets();
    //初始化信号与槽
    init_connections();
    //初始化用户自定义控件
    add_widgets_by_infos(self_widgets, sizeof(self_widgets) / sizeof(status_widgets_info));
    this->resize(1200, 700);

    this->setAutoFillBackground(true);
    window_bg.setBrush(QPalette::Background, QBrush(QPixmap(":/wbg.jpg")));
    this->setPalette(window_bg);
}
//根据描述结构体添加控件
void uart_gui::add_widget_by_info(status_widgets_info *swi)
{
    switch(swi->widget_type){
    case WTYPE_PUSHBOTTON:
            //由具体的串口类指定按钮的功能
            uhd->init_self_widgets(swi->index_in_arg, &sw.swis[sw.swi_count]);
            //设置按钮颜色
            set_pb_background_pic((QPushButton *)sw.swis[sw.swi_count].self_widget);

            add_widgets(sw.swis[sw.swi_count].self_widget);
            break;
    case WTYPE_CHECKBOX:
            break;

    case WTYPE_LABEL_TEXTLINE:
            //WTYPE_LABEL_TEXTLINE需从第一列开始，否则需占位
            if (count % 2){
                add_widgets(&space);
            }
            sw.swis[sw.swi_count].label = new QLabel(swi->widget_name);
            set_text_style(sw.swis[sw.swi_count].label);
            sw.swis[sw.swi_count].lineedit = new QLineEdit();
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
    set_ui_by_arg();
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
//初始化信号与曹
void uart_gui::init_connections()
{
    //串口选择改变
    connect(port_index, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_port_index_currentIndexChanged(QString)));
    //发送按钮
    connect(send, SIGNAL(clicked(bool)), this, SLOT(send_line_text()));
    //textbrowser 位置移到末尾
    connect(uart_log, SIGNAL(textChanged()), this, SLOT(text_browser_text_changed()));
    //打印log信息
    connect(uhd, SIGNAL(s_log_to_ui(QString, int)), this, SLOT(__log_to_ui(QString, int)));
    //点击设置参数按钮
    connect(set_arg, SIGNAL(clicked(bool)), this, SLOT(set_arg_by_ui()));
    //点击清屏参数按钮
    connect(clear, SIGNAL(clicked(bool)), this, SLOT(clear_browser()));
    //点击暂停参数按钮
    connect(pb_pause, SIGNAL(clicked(bool)), this, SLOT(pause_browser()));
    //点击记录到文件按钮
    connect(if_log_file, SIGNAL(clicked(bool)), this, SLOT(on_checkbox_log_to_file()));
    //处理串口过来的信号
    connect(uhd, SIGNAL(signal_for_uart_to_ui(uart_handler::signal_type)), this, SLOT(handle_uart_to_ui_signal(uart_handler::signal_type)));
}
void uart_gui::on_checkbox_log_to_file()
{
    if (if_log_file->isChecked()){
        init_log_file(  QDir::currentPath() + "/uart_gui.log");
        __log_to_ui("调试文件:" + log_file->fileName());
    }else{
        __log_to_ui("停止输出到文件");
        stop_log_file();
    }
}
void uart_gui::text_browser_text_changed()
{
    //static u_int8_t c = 0;
    uart_log->moveCursor(QTextCursor::End);
    //uart_log->setTextColor(QColor(rand() % 50, rand() % 100, rand() % 255));
}

void uart_gui::__log_to_ui(QString s, int type)
{
    QMutexLocker locker(&tb_mutex);
    QString __log;
    switch(type){
        case LOG_TYPE_INFO:
            uart_log->setTextColor(QColor(80, 238, 64));
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
            uart_log->setTextColor(QColor(242, 204, 198));
            uart_log->setFontPointSize(13);
            __log = "[DEBUG]";
            break;
        default:
            break;
    }
     __log.append("[" +QTime::currentTime().toString() + "]"+ s);
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
    port_index = new QComboBox;
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
    add_widgets(port_index);

    uart_stat = new QLabel;
    uart_stat->setText("未打开串口");
    set_text_style(uart_stat);
    add_widgets(uart_stat);

    send_text = new QLineEdit;
    set_lineedit_background_pic(send_text);
    add_widgets(send_text, true);

    send_hex = new QCheckBox("发送16进制");
    set_text_style(send_hex);
    add_widgets(send_hex);

    if_log_file = new QCheckBox("输出日志到文件");
    set_text_style(if_log_file);
    add_widgets(if_log_file);

    send = new QPushButton("发送");
    set_pb_background_pic(send);
    add_widgets(send);

    set_arg = new QPushButton("设置以下参数到设备");
    set_pb_background_pic(set_arg);
    add_widgets(set_arg);

    pb_pause = new QPushButton("暂停输出");
    set_pb_background_pic(pb_pause);
    add_widgets(pb_pause);

    clear = new QPushButton("清屏");
    set_pb_background_pic(clear);
    add_widgets(clear);

    uart_log = new QTextBrowser;
    QPalette palette=uart_log->palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
    uart_log->setPalette(palette);
    main_lay->addWidget(uart_log, 0, 1, 1, 1);
}
void uart_gui::on_port_index_currentIndexChanged(const QString &uart_port)
{
    bool x = uhd->open_serial_port(uart_port);
    if (x == true){
        uart_stat->setText("[开启成功！]");
        __log_to_ui("开启成功");
    }else{
        uart_stat->setText("[开启失败！]");
        __log_to_ui("开启失败");
        uhd->serial = NULL;
        return;
    }
    uhd->init_serial_param();
}
//向布局中添加一个控件
void uart_gui::add_widgets(QWidget *any_widgets, bool full_row)
{
    if (full_row == true){
        widgets_layout->insertRow(rows, any_widgets);
        count += 2;
    }else{
        widgets_layout->setWidget(rows, (QFormLayout::ItemRole)role, any_widgets);
        count++;
    }
    if (count % 2 == 0){
        rows++;
        role = QFormLayout::LabelRole;
    }else{
        role = QFormLayout::FieldRole;
    }
}

void uart_gui::init_layout()
{
    for_cert = new QWidget;
    main_lay = new QGridLayout;
    widgets_layout = new QFormLayout;

    main_lay->addLayout(widgets_layout, 0, 0, 1, 1);
    for_cert->setLayout(main_lay);
    this->setCentralWidget(for_cert);

    //控件列表与调试信息比例为1：3
    main_lay->setColumnStretch(0, 1);
    main_lay->setColumnStretch(1, 3);
}

uart_gui::~uart_gui()
{
    delete widgets_layout;
    delete main_lay;

    for (int i = 0; i < sw.swi_count; i++){
        delete sw.swis[i].swi;
    }
}

void uart_gui::handle_uart_to_ui_signal(uart_handler::signal_type t)
{
    switch (t){
        case uart_handler::ARG_CHANGED:
            set_ui_by_arg();
            break;
        default:
            break;
    }
}
