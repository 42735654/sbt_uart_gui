#ifndef UART_GUI_VM_H
#define UART_GUI_VM_H
#include "uart_gui.h"
#include "evm.h"
#include "uart_handler.h"
#include "select_bin_dg.h"
#include <QFileInfo>
class uart_gui_vm:public uart_gui, public evm
{
private:
    u_int8_t *vm_uart_stat_addr;

    QString last_bin_path;
    QString config_file_path = QString("uart-stat.dat");
    select_bin_dg select_bin_win;
    enum{
        VMCB_VM_SET_PARAM = 0,
        VMCB_UART_HANDLE = 1,
        VMCB_TOOLS = 2,
    };
    enum{
        UART_HANDLE_SEND = 0,
        UART_HANDLE_READ = 1,
        UART_HANDLE_READ_ALL = 2,
        UART_HANDLE_READLINE = 3,
        UART_HANDLE_PEEK = 4,
        UART_HANDLE_BYTES = 5,
    };
    enum{
        SET_PARAM_COMMON_INIT = 0,
        SET_PARAM_UI = 1,
        SET_PARAM_UART = 2,
    };
    enum{
        TOOL_LOG = 0,
        TOOL_REFRESH_UI = 1,
        TOOL_SET_LINEEDIT = 2,
        TOOL_DUMP_HEX = 3,
        TOOL_GET_TIME = 4,
    };
public:
    enum{
        CALL_VM_ID_UI_INIT = 1,
        CALL_VM_ID_UART_READ = 2,
        CALL_VM_ID_UI_CHANGED = 3,
    };

    uart_gui_vm(uart_handler *hd);
    void bin_close();
    bool bin_open(QString path = QString());
    virtual ~uart_gui_vm();
    void show_info();

protected:
    //QList<QString> binfile_list;
    QList<QFileInfo> binfile_list;
    void load_config(QString path = QString());
    void save_config(QString path = QString());
    void look_up_bin_in_dir(QString path);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    void set_config_file_path(QString path){
        config_file_path = path;
    }
    void show_all_apis();
    void show_change_logs();
    void on_open_bin(){
        if (bin_open()){
            set_ui_by_arg(vm_uart_stat_addr);
        }
    }
    void on_close_bin(){
        bin_close();
    }
    int32_t call_user(uint8_t funcid, uint8_t argc, int32_t *argv);
    int vm_tools_handle(uint8_t funcid, uint8_t argc, int32_t *argv);
    int vm_set_param(uint8_t funcid, uint8_t argc, int32_t *argv);
    int vm_uart_handle(uint8_t funcid, uint8_t argc, int32_t *argv);
    int do_evm_log(uint8_t argc, int32_t *argv);
    void call_vm_handle_uart(){
        call_vm_by_id(CALL_VM_ID_UART_READ);
    }
    void self_widgets_init(){
        call_vm_by_id(CALL_VM_ID_UI_INIT);
    }
    void ch_replace(char *a, char b, char c){
        for (int i = 0; i < strlen(a); i++){
            if (a[i] == b){
                a[i] = c;
            }
        }
    }
public slots:
    void handle_uart_data(){
        if (evm_ready()){
            call_vm_handle_uart();
        }else{
            uhd->serial->clear();
//            if (uhd->serial->canReadLine() || uhd->serial->bytesAvailable() > 100){
//                INFO("[Qt]%s", uhd->serial->readLine(500).replace('\r', ' ').replace('\n', ' ').data());
//            }
        }
    }
    void pushbotton_callback( int pushbotton_id){
            call_vm_by_id(pushbotton_id);
    }
    void on_set_pushed(){
        if (!evm_ready()){
            INFO("未加载虚拟机！");
            return;
        }
        set_arg_by_ui(vm_uart_stat_addr);
        call_vm_by_id(CALL_VM_ID_UI_CHANGED);
    }
    void on_config_save(){
        if (!evm_ready()){
            ERROR("并没有加载虚拟机");
            return;
        }
        set_arg_by_ui(vm_uart_stat_addr);
        memcpy(cfg.uart_stat_buf, vm_uart_stat_addr, sizeof(cfg.uart_stat_buf));
        save_config();
    }
    void reload_bin();
    void on_config_load(){
        if (!evm_ready()){
            ERROR("并没有加载虚拟机");
            return;
        }
        load_config();
        memcpy(vm_uart_stat_addr, cfg.uart_stat_buf, sizeof(cfg.uart_stat_buf));
        set_ui_by_arg(vm_uart_stat_addr);
    }
    void has_select_bin_file(QString filename);
};

#endif // UART_GUI_VM_H
