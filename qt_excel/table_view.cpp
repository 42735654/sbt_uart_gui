#include "table_view.h"

table_view::table_view(int x, int y)
{
    tb = new QTableWidget(y, x, this);
}

table_view::~table_view()
{
    if (tb){
        delete tb;
    }
}
 void table_view::add_row(int x, int y, QString s)
 {
    tb->setItem(y,x,new QTableWidgetItem(s));
 }
 void table_view::set_table_size(int x, int y)
 {
     tb->setColumnCount(x);
     tb->setRowCount(y);
 }
