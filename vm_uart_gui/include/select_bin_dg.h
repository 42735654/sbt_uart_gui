#ifndef SELECT_BIN_DG_H
#define SELECT_BIN_DG_H

#include <QDialog>

namespace Ui {
class select_bin_dg;
}

class select_bin_dg : public QDialog
{
    Q_OBJECT

public:
    explicit select_bin_dg(QWidget *parent = 0);
    ~select_bin_dg();
    void add_items(QStringList ql);
    void add_item(QString item);

private slots:
    void on_cancel_pb_clicked();
    void on_sure_pb_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

signals:
    void select_bin_file(QString filename);
private:
    Ui::select_bin_dg *ui;
};

#endif // SELECT_BIN_DG_H
