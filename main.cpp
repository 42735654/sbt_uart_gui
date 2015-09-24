#include "sbt_uart_handler.h"
#include "uart_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sbt_uart_handler  hd;
    uart_gui w(&hd);
    w.show();
    return a.exec();
}
