#include "uart_gui.h"
#include "type_def.h"
#include "config.h"
#include INCLUDE_FILE(INCLUDE_FILE_PART(DST_PROJECT, _uart_handler.h))
#include <QApplication>
//目前读取会有问题， 因此串口发送命令必须以\r\n结束
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uart_gui w(&hd);
    w.show();
    return a.exec();
}

