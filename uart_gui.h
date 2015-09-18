#ifndef UART_GUI_H
#define UART_GUI_H


class uart_gui
{
public:
    virtual void set_ui_by_arg() = 0;
    virtual void set_arg_by_ui();
    virtual void set_arg_by_uart();
    virtual void ui_refresh();
    virtual void uart_data_handle();
    virtual void send_uart_data();
};

#endif // UART_GUI_H
