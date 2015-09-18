#include "sbtwkq_uart_gui.h"
#include "ui_sbtwkq_uart_gui.h"

sbtwkq_uart_gui::sbtwkq_uart_gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sbtwkq_uart_gui)
{
    ui->setupUi(this);
}

sbtwkq_uart_gui::~sbtwkq_uart_gui()
{
    delete ui;
}
