#include "select_bin_dg.h"
#include "ui_select_bin_dg.h"

select_bin_dg::select_bin_dg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select_bin_dg)
{
    ui->setupUi(this);
    this->setWindowTitle("bin文件列表");
}

select_bin_dg::~select_bin_dg()
{
    delete ui;
}
void select_bin_dg::add_items(QStringList ql)
{
    ui->listWidget->addItems(ql);
}
void select_bin_dg::add_item(QString item)
{
    ui->listWidget->addItem(item);
}
void select_bin_dg::on_cancel_pb_clicked()
{
    ui->listWidget->clear();
    emit select_bin_file(QString(""));
    this->close();
}

void select_bin_dg::on_sure_pb_clicked()
{
    emit select_bin_file(ui->listWidget->currentItem()->text());
    ui->listWidget->clear();
    this->close();
}

void select_bin_dg::on_listWidget_doubleClicked(const QModelIndex &index)
{
    emit select_bin_file(ui->listWidget->currentItem()->text());
    ui->listWidget->clear();
    this->close();
}
