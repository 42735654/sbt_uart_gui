#include "uart_gui_vm.h"
#include <time.h>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMimeData>
#include <QListWidget>
#define NEED_PARAM(n)  do { \
    if (argc != n){  \
    ERROR("错误的调用：func id=%u, argc=%u, argv[0]=%u" ,funcid, argc, argv[0]); \
    return -1;   \
    }       \
    }while(0)

uart_gui_vm::~uart_gui_vm()
{

}
void uart_gui_vm::show_info()
{
    INFO("下位机模拟程序 ver%s, 编译时间:%s %s", VERSION, __DATE__, __TIME__);
    //INFO("请加载bin文件");
}
void uart_gui_vm::reload_bin()
{
    bin_open(last_bin_path);
}
void uart_gui_vm::show_all_apis()
{

}

void uart_gui_vm::show_change_logs()
{

}

uart_gui_vm::uart_gui_vm(uart_handler *hd):uart_gui(hd)
{
//    menu_add_action("打开bin文件", (CON_CALLBACK)&on_open_bin);
//    menu_add_action("卸载bin文件", (CON_CALLBACK)&on_close_bin);
//    menu_add_action("支持的虚拟机接口", (CON_CALLBACK)&show_all_apis);
    menu_add_action("版本信息", (CON_CALLBACK)&show_info);
//    menu_add_action("改动日志", (CON_CALLBACK)&show_change_logs);

    add_pb("保存配置", (CON_CALLBACK)&on_config_save);
    add_pb("读取配置", (CON_CALLBACK)&on_config_load);
    add_pb("加载bin文件", (CON_CALLBACK)&on_open_bin);
    add_pb("卸载bin文件", (CON_CALLBACK)&on_close_bin);
    add_pb("重载bin文件", (CON_CALLBACK)&reload_bin);
    connect(&select_bin_win, &select_bin_dg::select_bin_file, this, &has_select_bin_file);
    setAcceptDrops(true);
}
void uart_gui_vm::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void uart_gui_vm::look_up_bin_in_dir(QString path)
{
    QDir dir(path);
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            if (mfi.fileName().contains(".bin", Qt::CaseInsensitive) &&
                    mfi.size() <= 64 * 1024){
                binfile_list.push_front(mfi);
            }
        }else
        {
            if(mfi.fileName()=="." || mfi.fileName() == ".."){
                continue;
            }
            look_up_bin_in_dir(mfi.absoluteFilePath());
        }
    }
}
void uart_gui_vm::has_select_bin_file(QString filename)
{
    binfile_list.clear();
    if (!filename.isEmpty()){
        bin_open(filename);
    }
}

void uart_gui_vm::dropEvent(QDropEvent *event){
    QList<QUrl> u = event->mimeData()->urls();
    if(u.isEmpty())
        return;
    foreach(QUrl url, u) {
        QString file_name = url.toLocalFile();
        QFileInfo fi = QFileInfo(file_name);
        if (fi.isDir()){
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, "请确认",
                                                                   "搜索" + file_name + "?",
                                                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(rb == QMessageBox::No){
                break;
            }
            look_up_bin_in_dir(file_name);
            foreach(QFileInfo fi, binfile_list) {
                select_bin_win.add_item(fi.absoluteFilePath());
            }
            select_bin_win.exec();
        }else{
            if (bin_open(file_name)){
                set_ui_by_arg(vm_uart_stat_addr);
                break;
            }
        }
    }
    return;
}
void uart_gui_vm::bin_close()
{
    if (evm_ready() == false){
        return;
    }
    del_self_widgets();
    reset_evm();
    INFO("虚拟机卸载成功！");
}

bool uart_gui_vm::bin_open(QString path)
{
    if (!path.isEmpty()){
        last_bin_path = path;
    }else{
        last_bin_path = QFileDialog::getOpenFileName(this, tr("打开bin文件"),
                                                     last_bin_path, "虚拟机镜像(*.bin)");
    }
    if (last_bin_path.isEmpty()){
        return false;
    }else{
        bin_close();
        if (uhd && uhd->serial){
            uhd->serial->clear();
        }
        if (load_evm(last_bin_path)){
            load_config("./uart_stat.dat");
            memcpy(vm_uart_stat_addr, cfg.uart_stat_buf, sizeof(cfg.uart_stat_buf));
            set_evm_ready(true);
            self_widgets_init();
            set_ui_by_arg(vm_uart_stat_addr);
            return true;
        }
        return false;
    }
}

int uart_gui_vm::do_evm_log(uint8_t argc, int32_t *argv)
{
    int32_t i, val;
    u_int32_t num, len;
    int8_t *fmt, *tmp, buf[1024], buf_tmp[256];
    u_int16_t sfp_first;

    fmt = (int8_t *)get_vm_mem_addr()+ argv[0];
    sfp_first = argv[1];
    memset(buf, 0, sizeof(buf));
    num = 0;
    len = strlen((const char *)fmt);
    for(i=0; i<len; i++){
        if(fmt[i] == '%' && fmt[i+1] == 's'){
            tmp = (int8_t *)get_vm_mem_addr()+ mem_read(sfp_first + 4*num, IS_32B);
            strcat((char *)buf, (const char *)tmp);
            num++;
            i++;
        }
        else if(fmt[i] == '%' && fmt[i+1] == 'd'){
            memset((char *)buf_tmp, 0, sizeof(buf_tmp));
            val = mem_read(sfp_first + 4*num, IS_32B);
            sprintf((char *)buf_tmp, "%d", val);
            strcat((char *)buf, (const char *)buf_tmp);
            num++;
            i++;
        }
        else{
            buf[strlen((char *)buf)] = fmt[i];
        }
    }
    len = strlen((char *)buf) +1;
    num = 0;
    while(len > 240){//长度过长分多长打印
        memset((u_int8_t*)buf_tmp, 0, sizeof(buf_tmp));
        memcpy((u_int8_t *)buf_tmp, (u_int8_t *)buf+num*240, 240);
        ch_replace((char *)buf_tmp, '\n', ' ');
        ch_replace((char *)buf_tmp, '\r', ' ');
        VM_INFO("%s",  buf_tmp);
        num++;
        len -= 240;
    }
    ch_replace((char *)(buf+num*240), '\n', ' ');
    ch_replace((char *)(buf+num*240), '\r', ' ');
    VM_INFO("%s", buf+num*240);
    return 0;
}
void uart_gui_vm::save_config(QString path)
{
    if (path.isEmpty()){
        set_config_file_path(QFileDialog::getSaveFileName(this, tr("保存配置文件"),     \
                                                          config_file_path, "配置文件(*.dat)"));
    }
    if (config_file_path.isEmpty()){
        return;
    }
    QFile config_file(config_file_path);
    if (config_file.open(QIODevice::WriteOnly) != true){
        ERROR("无法打开配置文件[%s],保存失败", config_file_path.toLatin1().data());
        return;
    }
    int n =  config_file.write((char *)&cfg, sizeof(uart_config));

    if (n < sizeof(uart_config)){
        ERROR("写配置文件出错，请求%d字节，写入%d字节", sizeof(uart_config), n);
    }else{
        config_file.flush();
        INFO("保存配置成功！");
    }
    //   INFO(" save cfg.uart_stat_buf[0]=%d", cfg.uart_stat_buf[0]);
    config_file.close();
}

void uart_gui_vm::load_config(QString path)
{
    if (path.isEmpty()){
        set_config_file_path(QFileDialog::getOpenFileName(this, tr("打开配置文件"),
                                                          config_file_path, "配置文件(*.dat)"));
    }
    if (config_file_path.isEmpty()){
        return;
    }
    QFile config_file(config_file_path);
    if (config_file.open(QIODevice::ReadOnly) != true){
        INFO("打开配置文件[%s]失败", config_file_path.toLatin1().data());
        return;
    }
    int n = config_file.read((char *)&cfg, sizeof(uart_config));
    if (n != sizeof(uart_config)){
        ERROR("读取配置文件错误！请求%d字节，读取到%d字节", sizeof(uart_config), n);
        return;
    }
    config_file.close();
    INFO("读取配置成功！");
    //    INFO("load cfg.uart_stat_buf[0]=%d", cfg.uart_stat_buf[0]);
}
int uart_gui_vm::vm_tools_handle(uint8_t funcid, uint8_t argc, int32_t *argv)
{
    if (argc < 1){
        return -1;
    }
    int ret = 0;
    QByteArray hex;
    switch (argv[0]){
    case TOOL_LOG:
        ret = do_evm_log(argc - 1, argv + 1);
        break;
    case TOOL_REFRESH_UI:
        if (memcmp(vm_uart_stat_addr, cfg.last_uart_stat_buf, sizeof(cfg.last_uart_stat_buf))){
            set_ui_by_arg(vm_uart_stat_addr);
            memcpy(cfg.last_uart_stat_buf, vm_uart_stat_addr, sizeof(cfg.last_uart_stat_buf));
        }
        break;
    case TOOL_GET_TIME:
        ret = time(NULL);
        break;
    case TOOL_SET_LINEEDIT:
        NEED_PARAM(3);
        for (int i = 0; i < sw.swi_count; i++){
            if (sw.swis[i].swi->index_in_arg == argv[1]){
                switch (sw.swis[i].swi->widget_type){
                    case WTYPE_LABEL_TEXTLINE:
                        ((QLineEdit *)sw.swis[i].lineedit)->setText((char *)(get_vm_mem_addr()+ argv[2]));
                        break;
                case WTYPE_PUSHBOTTON:
                         ((QPushButton *)sw.swis[i].self_widget)->setText((char *)(get_vm_mem_addr()+ argv[2]));
                        break;
                default:
                    break;

                }

//                if (sw.swis[i].swi->widget_type == WTYPE_LABEL_TEXTLINE){
//                    WARN("不能设置该控件！");
//                }else if (){

//                }
                break;
            }
        }
        break;
    case TOOL_DUMP_HEX:
        hex.append((char *)(get_vm_mem_addr() + argv[1]), argv[2]);
        INFO("%s", hex.toHex().data());
        break;
    default:
        break;
    }
    return ret;
}
int uart_gui_vm::vm_set_param(uint8_t funcid, uint8_t argc, int32_t *argv)
{
    if (argc < 1){
        return -1;
    }
    int ret = 0;
    status_widgets_info vm_swi;
    char *data;
    switch (argv[0]){
    case SET_PARAM_COMMON_INIT:
        NEED_PARAM(3);
        set_call_id_addr(get_vm_mem_addr()+ argv[1]);
        vm_uart_stat_addr = (get_vm_mem_addr()+ argv[2]);
        break;
    case SET_PARAM_UI:
        //INFO("SET_PARAM_UI:%d,argc=%u",funcid, argc);
        NEED_PARAM(2);
        data = (char *)(get_vm_mem_addr()+ argv[1]);
        vm_swi.widget_type = data[0];
        vm_swi.index_in_arg = data[1];
        vm_swi.widget_name = &data[2];
        add_widgets_by_infos(&vm_swi, 1);
        break;
    case SET_PARAM_UART:
        NEED_PARAM(2);
        uhd->btl = argv[1];
        INFO("波特率设置为%d", argv[1]);
        break;
    default:
        break;
    }
    return ret;
}

int uart_gui_vm::vm_uart_handle(uint8_t funcid, uint8_t argc, int32_t *argv)
{
    if (argc < 1){
        return -1;
    }
    char *data;
    if (argc > 1){
        data = (char *)(get_vm_mem_addr()+ argv[1]);
    }
    qint64 ret = 0;
    switch (argv[0]){
    case UART_HANDLE_SEND:
        NEED_PARAM(3);
        ret = uhd->uart_send(get_vm_mem_addr()+ argv[1], argv[2]);
        break;
    case UART_HANDLE_READ:
        ret = uhd->serial->read(data, argv[2]);
        NEED_PARAM(3);
        break;
    case UART_HANDLE_READ_ALL:
        ret = uhd->serial->read(data, uhd->serial->bytesAvailable());
        NEED_PARAM(3);
        break;
    case UART_HANDLE_READLINE:
        ret = uhd->serial->readLine(data, argv[2]);
        NEED_PARAM(3);
        break;
    case UART_HANDLE_PEEK:
        NEED_PARAM(3);
        ret = uhd->serial->peek(data, argv[2]);
        break;
    case UART_HANDLE_BYTES:
        NEED_PARAM(1);
        ret = uhd->serial->bytesAvailable();
        break;
    default:
        break;
    }
    return ret;
}


int32_t uart_gui_vm::call_user(uint8_t funcid, uint8_t argc, int32_t *argv)
{
    int ret = 0;
    switch(funcid){
    case VMCB_VM_SET_PARAM:
        ret = vm_set_param(funcid, argc, argv);
        break;
    case VMCB_UART_HANDLE:
        ret = vm_uart_handle(funcid, argc, argv);
        break;
    case VMCB_TOOLS:
        ret = vm_tools_handle(funcid, argc, argv);
        break;
    default:
        INFO("未知回调函数ID:%d,argc=%u",funcid, argc);
        break;
    }
    return ret;
}
