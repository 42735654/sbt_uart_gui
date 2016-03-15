#include "mainwindow.h"
#include <QApplication>
#include <qaxobject.h>
#include <QAxWidget>
#include <qDebug>
#include "table_view.h"
#include "block_data.h"
#include "excel_handler.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    excel_handler eh("F:\\1.xlsx");
    return a.exec();
}
