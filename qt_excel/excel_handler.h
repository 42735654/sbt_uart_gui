#ifndef EXCEL_HANDLER_H
#define EXCEL_HANDLER_H

#include <QAxObject>
#include <QAxWidget>

class excel_handler
{
private:
    char ***data;
public:
    excel_handler();
    void load_from_file(QString file);
    void get_info_list(int x, int y);
};

#endif // EXCEL_LOADER_H
