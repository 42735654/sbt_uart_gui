#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <time.h>
#include <QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menu =menuBar()->addMenu("菜单");
    menu_add_action("加载excel", on_open_excel);
    ready = false;
}
void MainWindow::on_open_excel()
{
    last_excel_path = QFileDialog::getOpenFileName(this, tr("打开excel"), last_excel_path, "电子表格文件(*.xlsx *.xls)");
    if (last_excel_path.isEmpty()){
        return;
    }
    sheets_datas = eh.load_excel_file(last_excel_path);

    init_student_list_by_sheet(eh.sheets_data, eh.sheets_count());
}

void MainWindow::menu_add_action(const char *name, CON_CALLBACK cb)
{
    QAction *act = new QAction(name, this);
    connect(act, &QAction::triggered, this, cb);
    menu->addAction(act);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::init_student_list_by_sheet(sheet **sheets, int n)
{
    QRect *rect;
    ui->list_table->clear();
    table = new QTableWidget *[n];
    for (int i = 0; i < n; i++){
        rect = sheets[i]->sheet_info;
        table[i] = new QTableWidget(rect->height(), rect->width());
        ui->list_table->insertTab(i, table[i], QString("班级") + QString::number(i + 1));
        for (int j = rect->x(); j < rect->width() + rect->x(); j++){
            for (int k = rect->y(); k < rect->height() + rect->y(); k++){
                table[i]->setItem(k - rect->y(), j - rect->x(),
                                  new QTableWidgetItem(sheets[i]->get_memsheet_value(j, k)));
            }
        }
    }
    ready = true;
}

void MainWindow::on_every_pb_clicked()
{
    ui->func_table->setCurrentIndex(1);
}

void MainWindow::on_anyone_pb_clicked()
{
    ui->func_table->setCurrentIndex(2);
}

void MainWindow::on_rand_pb_clicked()
{
    if (!ready){
        return;
    }
    sheet *st = sheets_datas[ui->list_table->currentIndex()];
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int n = qrand() % (st->sheet_info->height() - 1) + 2;
    ui->rand_name_label->setText(st->get_memsheet_value(1, n));
    ui->rand_num_label->setText(st->get_memsheet_value(2, n));
}
