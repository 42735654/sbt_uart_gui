#include "sheet.h"

sheet::sheet()
{

}

sheet::sheet(int x_start, int y_start, int x_count, int y_count)
{
    sheet_info = new QRect(x_start, y_start, x_count, y_count);
    data = new block_data<QString>(x_count, y_count);
}
sheet::~sheet()
{
    delete sheet_info;
    delete data;
}
