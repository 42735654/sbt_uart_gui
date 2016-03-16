#ifndef EXCEL_HANDLER_H
#define EXCEL_HANDLER_H

#include <QAxObject>
#include <QAxWidget>
#include <qDebug>
#include <QRect>
#include <QDir>
#include "block_data.h"
#include "sheet.h"

class excel_handler
{
private:
    QAxObject *excel;
    QAxObject *work_books, *cur_work_book;
    QAxObject *sheets;
    int sheet_count;
    QString last_file_path;
    bool ready;
    void init_excel_env();

protected:

public:
    sheet **sheets_data;
    excel_handler();
    ~excel_handler();
    excel_handler(QString file);
    int sheets_count(){
        return sheet_count;
    }
    sheet ** load_excel_file(QString file);
    QRect get_excel_sheet_rect(int index);
    void set_excel_value(int index, int x, int y, QVariant value);
    QVariant get_excel_value(int index, int x, int y);
    void save_to_file(QString file = "");
    void close_excel();

    //待实现
    void set_sheet_name(int index, QString name);
    void add_sheet();


};

#endif // EXCEL_LOADER_H
