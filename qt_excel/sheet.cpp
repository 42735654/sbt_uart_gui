#include "sheet.h"

sheet::sheet()
{

}

sheet::sheet(int x_start, int y_start, int x_count, int y_count):
    block_data<QString>(x_count, y_count)
{
    sheet_info = new QRect(x_start, y_start, x_count, y_count);
}

sheet::sheet(QRect rect):block_data<QString>(rect.width(), rect.height())
{
    sheet_info = new QRect(rect);
}

sheet::~sheet()
{
    delete sheet_info;
    delete data;
}
void sheet::set_memsheet_value(int px, int py, QString value)
{
    data[px - sheet_info->x()][py - sheet_info->y()] = value;
}
QString  sheet::get_memsheet_value(int px, int py)
{
    return data[px - sheet_info->x()][py - sheet_info->y()];
}
