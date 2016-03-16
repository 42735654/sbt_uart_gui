#include "excel_handler.h"

void excel_handler::init_excel_env()
{
    excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", true);
    excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);
    ready = false;
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
//    save_to_file();
    close_excel();
    delete excel;
}
//获取表格使用过的范围
QRect excel_handler::get_excel_sheet_rect(int index)
{
    if (!ready){
        return QRect(0, 0, 0, 0);
    }
    QAxObject *sheet = cur_work_book->querySubObject("Sheets(int)", index);
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
void excel_handler::save_to_file(QString file)
{
    if (!ready){
        return;
    }
    if (file.isEmpty()){
        file = last_file_path;
    }
    cur_work_book->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(file));
}
void excel_handler::close_excel()
{
    if (!ready){
        return;
    }
    ready = false;
    cur_work_book->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel
}
void excel_handler::set_excel_value(int index, int x, int y, QVariant value)
{
    if (index < 1 || index > sheets_count() || !ready){
        return;
    }
    QAxObject *dst_sheet = cur_work_book->querySubObject("Sheets(int)", index);
    QAxObject *dst_cell = dst_sheet->querySubObject("Cells(int,int)", y, x);
    dst_cell->dynamicCall("SetValue(const QVariant&)",value);
    dst_cell->setProperty("WrapText", true);
//    qDebug("h=%d,w=%d", dst_cell->property("RowHeight").toInt(),
//           dst_cell->property("ColumnWidth").toInt());
//    dst_cell->setProperty("RowHeight", value.value<QString>().length());  //设置单元格行高
//    dst_cell->setProperty("ColumnWidth", value.value<QString>().length() * 2);  //设置单元格列宽
//    qDebug("h=%d,w=%d", dst_cell->property("RowHeight").toInt(),
//           dst_cell->property("ColumnWidth").toInt());
}

QVariant excel_handler::get_excel_value(int index, int x, int y)
{
    if (index < 1 || index > sheets_count() || !ready){
        return 0;
    }
    QAxObject *temp_sheet = cur_work_book->querySubObject("Sheets(int)", index);
    QAxObject *cell = temp_sheet->querySubObject("Cells(int,int)", y, x);
    return cell->property("Value");  //获取单元格内容
}

sheet ** excel_handler::load_excel_file(QString file)
{
    if (file.isEmpty()){
        return 0;
    }
    last_file_path = file;
    work_books = excel->querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", file);
    cur_work_book = excel->querySubObject("ActiveWorkBook");
    sheets = cur_work_book->querySubObject("Sheets");
    sheet_count = sheets->property("Count").toInt();
    ready = true;
    sheets_data = new sheet *[sheet_count];
    QRect rect;

    for (int k = 0; k < sheet_count; k++){
        rect = get_excel_sheet_rect(k + 1);
        sheets_data[k] = new sheet(rect);
        for(int i = rect.x(); i < rect.width() + rect.x(); i++)
        {
            for(int j = rect.y(); j < rect.y() + rect.height(); j++)
            {
                QVariant cell_value = get_excel_value(k + 1, i, j);
                sheets_data[k]->set_memsheet_value(i, j, cell_value.value<QString>());
            }
        }
    }
    return sheets_data;
}
