#include "sbt_uart_handler.h"
#include "uart_gui.h"
#include <QApplication>
//目前读取会有问题， 因此串口发送命令必须以\r\n结束
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sbt_uart_handler  hd;
    uart_gui w(&hd);
    w.show();
    return a.exec();
}
