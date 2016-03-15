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

excel_handler::~excel_handler()
{
    for (int i = 0; i < sheet_count; i++){
        delete sheets_data[i];
    }
    delete sheets_data;
}
//获取表格使用过的范围
QRect excel_handler::get_excel_sheet_rect(QAxObject *sheet)
{
    QAxObject *used_range = sheet->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");
    int row_start = used_range->property("Row").toInt();  //获取起始行
    int column_start = used_range->property("Column").toInt();  //获取起始列
    int row_count = rows->property("Count").toInt();  //获取行数
    int column_count = columns->property("Count").toInt();  //获取列数
    QRect sheet_rect(column_start, row_start, column_count, row_count);
    return sheet_rect;
}

void excel_handler::load_excel_file(QString file)
{
    last_file_path = file;
    work_books = excel->querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", file);
    cur_work_book = excel->querySubObject("ActiveWorkBook");
    sheets = cur_work_book->querySubObject("Sheets");
    sheet_count = sheets->property("Count").toInt();
    sheets_data = new sheet *[sheet_count];
    QRect rect;
    //遍历同一文件中所有工作表
    for (int k = 0; k < sheet_count; k++){
        QAxObject *temp_sheet = cur_work_book->querySubObject("Sheets(int)", k + 1);
        rect = get_excel_sheet_rect(temp_sheet);
        sheets_data[k] = new sheet(rect);
        qDebug("x=%d,y=%d,xs=%d,ys=%d", rect.x(), rect.y(), rect.width(), rect.height());
        for(int i = rect.x(); i < rect.width() + rect.x(); i++)
        {
            for(int j = rect.y(); j < rect.y() + rect.height(); j++)
            {
                QAxObject *cell = temp_sheet->querySubObject("Cells(int,int)", j, i);
                QVariant cell_value = cell->property("Value");  //获取单元格内容
                sheets_data[k]->set_memsheet_value(i, j, cell_value.value<QString>());
                qDebug()<<k<<".<"<<i<<","<<j<<">"<<sheets_data[k]->get_memsheet_value(i, j);
            }
        }
    }
    cur_work_book->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel
}
