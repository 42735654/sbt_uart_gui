#include "config.h"
#include "uart_handler.h"
#include "evm.h"
#include "uart_gui_vm.h"
#include "uart_gui.h"
#include "type_def.h"
#include <QApplication>
uart_gui *p;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uart_handler hd;
    uart_gui_vm t(&hd);
    p = &t;
    t.show_info();
    t.show();

    return a.exec();
}

