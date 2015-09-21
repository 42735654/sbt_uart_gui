#include "sbtwkq_uart_gui.h"
#include "sbt_uart_handler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sbt_uart_handler  hd(QSerialPort::Baud9600);
    uart_gui w(&hd);

    w.add_widgets_by_infos(sbt_self_widgets, sizeof(sbt_self_widgets) / sizeof(status_widgets_info));

    w.show();
    return a.exec();
}
