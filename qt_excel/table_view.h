#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <QWidget>
#include <QTableWidget>
class table_view : public QWidget
{
    Q_OBJECT

public:
    table_view(int x, int y);
    ~table_view();
    void add_row(int x, int y, QString s);
    void set_table_size(int x, int y);

private:
    QTableWidget *tb;
};

#endif // TABLE_VIEW_H
