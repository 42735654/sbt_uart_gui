#if defined DEV_SBT
#include "sbt_uart_handler.h"
#elif defined DEV_CAMLIGHT
#include "camlight_uart_handler.h"
#elif defined DEV_SXDKL
#include "sxdkl_uart_handler.h"
#endif

#include "uart_gui.h"
#include <QApplication>
//目前读取会有问题， 因此串口发送命令必须以\r\n结束
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uart_gui w(&hd);
    w.show();
    return a.exec();
}

