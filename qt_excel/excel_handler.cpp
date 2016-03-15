#include "excel_handler.h"


void excel_handler::init_excel_env()
{
    excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", true);
    excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);
}

excel_handler::excel_handler()
{
    init_excel_env();
}

excel_handler::excel_handler(QString file)
{
     init_excel_env();
    load_excel_file(file);
}
void excel_handler::delete_sheet(int sheet_index)
{
    delete sheets_data[sheet_index];
}
excel_handler::~excel_handler()
{
    for (int i = 0; i < sheet_count; i++){
        delete_sheet(i);
    }
    delete sheets_data;
}

void excel_handler::load_excel_file(QString file)
{
    work_books = excel->querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", file);
    cur_work_book = excel->querySubObject("ActiveWorkBook");
    sheets = cur_work_book->querySubObject("Sheets");
    sheet_count = sheets->property("Count").toInt();
    sheets_data = new sheet *[sheet_count];

    for (int k = 0; k < sheet_count; k++){
        QAxObject *temp_sheet = cur_work_book->querySubObject("Sheets(int)", k + 1);
        QAxObject *used_range = temp_sheet->querySubObject("UsedRange");
        QAxObject *rows = used_range->querySubObject("Rows");
        QAxObject *columns = used_range->querySubObject("Columns");
        int row_start = used_range->property("Row").toInt();  //获取起始行
        int column_start = used_range->property("Column").toInt();  //获取起始列
        int row_count = rows->property("Count").toInt();  //获取行数
        int column_count = columns->property("Count").toInt();  //获取列数
        sheets_data[k] = new sheet(column_start, row_start,
                                   column_count, row_count);
        for(int i = row_start; i < row_count; i++)
        {
            for(int j = column_start; j < column_count; j++)
            {
                QAxObject *cell = temp_sheet->querySubObject("Cells(int,int)", i, j);
                QVariant cell_value = cell->property("Value");  //获取单元格内容
                sheets_data[k]->data->data[i][j] = cell_value.value<QString>();
                qDebug()<<sheets_data[k]->data->data[i][j];
            }
        }
    }
    cur_work_book->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel
}
