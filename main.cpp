#include "sbtwkq_uart_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sbtwkq_uart_gui w;
    w.show();

    return a.exec();
}
