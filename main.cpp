#include "sbt_uart_handler.h"
#include "uart_gui.h"
#include <QApplication>
status_widgets_info  sbt_self_widgets[] = {
    {0, "温度"},
    {1, "风速"},
    {2, "模式"},
    {3, "开关机"},
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sbt_uart_handler  hd;
    uart_gui w(&hd);
    qDebug("%d", GET_INDEX_BY_NAME(uart_stat_arg, temp));
    w.add_widgets_by_infos(sbt_self_widgets, sizeof(sbt_self_widgets) / sizeof(status_widgets_info));

    w.show();
    return a.exec();
}
