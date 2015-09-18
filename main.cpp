#include "sbtwkq_uart_gui.h"
#include "sbt_uart_handler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sbt_uart_handler hd;
    sbtwkq_uart_gui *w = new sbtwkq_uart_gui(&hd);
    w->show();

    return a.exec();
}
