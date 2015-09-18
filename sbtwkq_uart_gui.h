#ifndef SBTWKQ_UART_GUI_H
#define SBTWKQ_UART_GUI_H

#include <QMainWindow>
#include "uart_gui.h"
#include "uart_handler.h"
#include "sbt_uart_handler.h"

class sbtwkq_uart_gui : public uart_gui
{
    Q_OBJECT

public:
    explicit sbtwkq_uart_gui(uart_handler *hd);
    ~sbtwkq_uart_gui();

public:
    void set_ui_by_arg();
    void set_arg_by_ui();
    void set_arg_by_uart();
    void ui_refresh();
    void uart_data_handle();
    void send_uart_data();
};

#endif // SBTWKQ_UART_GUI_H
