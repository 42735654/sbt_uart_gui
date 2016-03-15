#ifndef SHEET_H
#define SHEET_H

#include "block_data.h"
#include <QString>
#include <QRect>
class sheet
{
public:
    block_data<QString> *data;
    sheet();
    sheet(int x_start, int y_start, int x_count, int y_count);
    ~sheet();
    QRect *sheet_info;
};

#endif // SHEET_H
