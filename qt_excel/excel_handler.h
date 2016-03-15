#ifndef EXCEL_HANDLER_H
#define EXCEL_HANDLER_H

#include <QAxObject>
#include <QAxWidget>
#include <qDebug>
#include "block_data.h"
#include "sheet.h"

class excel_handler
{
private:
    sheet **sheets_data;
    QAxObject *excel;
    QAxObject *work_books, *cur_work_book;
    QAxObject *sheets, *cur_sheet;
    int sheet_count;

    void init_excel_env();
protected:

public:
    excel_handler();
    ~excel_handler();
    excel_handler(QString file);
    void delete_sheet(int sheet_index);
    void load_excel_file(QString file);
//    void get_info_list(int x, int y);
};

#endif // EXCEL_LOADER_H
