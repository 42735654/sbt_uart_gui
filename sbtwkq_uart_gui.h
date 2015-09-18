#ifndef SBTWKQ_UART_GUI_H
#define SBTWKQ_UART_GUI_H

#include <QMainWindow>
#include "uart_gui.h"
namespace Ui {
class sbtwkq_uart_gui;
}

class sbtwkq_uart_gui : public QMainWindow, public uart_gui
{
    Q_OBJECT

public:
    explicit sbtwkq_uart_gui(QWidget *parent = 0);
    ~sbtwkq_uart_gui();

public:
    void set_ui_by_arg();
    void set_arg_by_ui();
    void set_arg_by_uart();
    void ui_refresh();
    void uart_data_handle();
    void send_uart_data();
private:
    Ui::sbtwkq_uart_gui *ui;
};

#endif // SBTWKQ_UART_GUI_H
