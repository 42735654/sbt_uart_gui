#include "uart_gui.h"
uart_gui::uart_gui(uart_handler *hd)
{
    uhd = hd;
    rows = 0;
    count = 0;
    role = QFormLayout::LabelRole;
    init_widgets();
    init_layout();
    this->resize(800, 480);
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
    uart_log = new QTextBrowser;

    main_lay = new QGridLayout;
    for_cert = new QWidget;
    widgets_layout = new QFormLayout;
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

    for_cert->setLayout(main_lay);
    this->setCentralWidget(for_cert);
    main_lay->addWidget(uart_log, 0, 1, 1, 1);
    //比例为1：3
    main_lay->setColumnStretch(0, 1);
    main_lay->setColumnStretch(1, 3);
}

uart_gui::~uart_gui()
{
    delete port_index;
    delete uart_stat;
    delete send_text;
    delete send;
    delete uart_log;
}
