#include "excel_handler.h"

excel_handler::excel_handler()
{

}

void excel_handler::get_info_list(int x, int y)
{
    data = (char ***)new char *[x * y];
}
