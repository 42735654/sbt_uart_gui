#include "sbt_uart_handler.h"
#include "uart_gui.h"
#include <QApplication>
status_widgets_info  sbt_self_widgets[] = {
    {GET_SBT_INDEX( temp), "温度"},
    {GET_SBT_INDEX(wind), "风速"},
    {GET_SBT_INDEX(mode), "模式"},
    {GET_SBT_INDEX(power), "开关机"},

    {GET_SBT_INDEX(auto_mode), "自动模式"},
    {GET_SBT_INDEX(temp_makeup), "温度补偿值"},
    {GET_SBT_INDEX(low_temp), "低温保护温度"},
    {GET_SBT_INDEX(wind_power), "阀关模式控制"},

    {GET_SBT_INDEX(temp_rd), "温控器回差温度"},
    {GET_SBT_INDEX(low_mode), "低温保护模式"},
    {GET_SBT_INDEX(max_temp), "限制制热最高温"},
    {GET_SBT_INDEX(min_temp), "限制制冷最低温"},
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
