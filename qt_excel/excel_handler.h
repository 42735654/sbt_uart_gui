#ifndef EXCEL_HANDLER_H
#define EXCEL_HANDLER_H

#include <QAxObject>
#include <QAxWidget>
#include <qDebug>
#include <QRect>
#include "block_data.h"
#include "sheet.h"

class excel_handler
{
private:
    sheet **sheets_data;
    QAxObject *excel;
    QAxObject *work_books, *cur_work_book;
    QAxObject *sheets;
    int sheet_count;
    QString last_file_path;

    void init_excel_env();
    QRect get_excel_sheet_rect(QAxObject *sheet);
protected:

public:
    excel_handler();
    ~excel_handler();
    excel_handler(QString file);
    void load_excel_file(QString file);


    //待实现
    void set_sheet_name(int index, QString name);
    void add_sheet();

    void set_excel_value(int index, int x, int y, QString value){
        QAxObject *dst_sheet = sheets->querySubObject("Item(int)", index);
        QAxObject *dst_cell = dst_sheet->querySubObject("Cells(int,int)", y, x);
        dst_cell->setProperty("Value", value.toLatin1().data());  //设置单元格值
    }



//    void get_info_list(int x, int y);
};

#endif // EXCEL_LOADER_H
