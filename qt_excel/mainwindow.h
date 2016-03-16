#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<sheet.h>
#include <QRect>
#include <QTableWidget>
#include <QMenu>
#include <QAction>
#include "excel_handler.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    bool ready;

public:
    excel_handler eh;
    QTableWidget **table;
    QMenu *menu;
    QString last_excel_path;
    sheet **sheets_datas;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init_student_list_by_sheet(sheet **sheets, int n);
    typedef void (MainWindow::*CON_CALLBACK)();
    void menu_add_action(const char *name, CON_CALLBACK cb);
    void on_open_excel();

private slots:
    void on_every_pb_clicked();

    void on_anyone_pb_clicked();

    void on_rand_pb_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
