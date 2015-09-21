#include "uart_gui.h"
void uart_gui::set_ui_by_arg()
{
    u_int8_t n, index;
    for (int i = 0; i < sw.swi_count; i++){
        index = (sw.swis[i].swi)->index_in_arg;
        n = uhd->arg[index];
        sw.swis[i].lineedit->setText(QString::number(n));
    }
}

void uart_gui::set_arg_by_ui()
{
    u_int8_t n, index;
    for (int i = 0; i < sw.swi_count; i++){
        index = (sw.swis[i].swi)->index_in_arg;
        n = sw.swis[i].lineedit->text().toInt();
        uhd->arg[index] = n;
    }
}
uart_gui::uart_gui(uart_handler *hd)
{
    uhd = hd;
    rows = 0;
    count = 0;
    sw.swi_count = 0;
    role = QFormLayout::LabelRole;
    init_widgets();
    init_connections();
    init_layout();
    this->resize(800, 480);
    init_serial_port();
}
void uart_gui::add_widget_by_info(status_widgets_info *swi)
{
    if (count % 2){
        add_widgets(&space);
    }
    sw.swis[sw.swi_count].label = new QLabel(swi->widget_name);
    sw.swis[sw.swi_count].lineedit = new QLineEdit();

    add_widgets(sw.swis[sw.swi_count].label);
    add_widgets(sw.swis[sw.swi_count].lineedit);
    sw.swi_count++;

}
void uart_gui::add_widgets_by_infos(status_widgets_info *swi, int count)
{
    for (int i = 0; i < count; i++){
        sw.swis[sw.swi_count].swi = new status_widgets_info;
        memcpy(sw.swis[sw.swi_count].swi, &swi[i], sizeof(status_widgets_info));
        add_widget_by_info(sw.swis[sw.swi_count].swi);
    }
}
void uart_gui::init_connections()
{
    //串口选择改变
    connect(port_index, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_port_index_currentIndexChanged(QString)));
    //发送按钮
    connect(send, SIGNAL(clicked(bool)), this, SLOT(send_line_text()));

    connect(uart_log, SIGNAL(textChanged()), this, SLOT(text_browser_text_changed()));

    connect(uhd, SIGNAL(log_to_ui(QString)), this, SLOT(__log_to_ui(QString)));

    connect(set_arg, SIGNAL(clicked(bool)), this, SLOT(set_arg_by_ui()));

}
void uart_gui::text_browser_text_changed()
{
    uart_log->moveCursor(QTextCursor::End);
}

void uart_gui::__log_to_ui(QString s)
{
    uart_log->append(s);
}

void uart_gui::send_line_text()
{
    QString t = send_text->text();
    uhd->uart_send(t.toLatin1().data(), t.length());
}

void uart_gui::init_widgets()
{
    port_index = new QComboBox;
    port_index->addItem("串口选择");
    uart_stat = new QLabel;
    uart_stat->setText("串口关闭");
    send_text = new QLineEdit;
    send = new QPushButton;
    send->setText("发送");
    set_arg = new QPushButton;
    set_arg->setText("设置以下参数到设备");
    uart_log = new QTextBrowser;
    sw.swi_count = 0;
    main_lay = new QGridLayout;
    for_cert = new QWidget;
    widgets_layout = new QFormLayout;
}
void uart_gui::init_serial_port()
{
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        port_index->addItem("没有可用串口");
        return;
    }
    foreach (QSerialPortInfo info, infos) {
        port_index->addItem(info.portName());
    }
}
void uart_gui::on_port_index_currentIndexChanged(const QString &arg1)
{
    bool x = uhd->open_serial_port(arg1);
    if (x == true){
        uart_stat->setText("[开启成功！]");
        __log_to_ui("开启成功");
    }else{
        uart_stat->setText("[开启失败！]");
        __log_to_ui("开启失败");
        return;
    }
    uhd->init_serial_param();
}
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
    main_lay->addLayout(widgets_layout, 0, 0, 1, 1);
    add_widgets(uart_stat);
    add_widgets(port_index);
    add_widgets(send_text, true);
    add_widgets(send);
    add_widgets(set_arg);

    for_cert->setLayout(main_lay);
    this->setCentralWidget(for_cert);
    main_lay->addWidget(uart_log, 0, 1, 1, 1);
    //比例为1：3
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
