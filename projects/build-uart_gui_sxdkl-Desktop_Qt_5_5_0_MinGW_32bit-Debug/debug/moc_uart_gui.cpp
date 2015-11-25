/****************************************************************************
** Meta object code from reading C++ file 'uart_gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../work_code/vm_uart_gui/include/uart_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uart_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_uart_gui_t {
    QByteArrayData data[17];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_uart_gui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_uart_gui_t qt_meta_stringdata_uart_gui = {
    {
QT_MOC_LITERAL(0, 0, 8), // "uart_gui"
QT_MOC_LITERAL(1, 9, 33), // "on_port_index_currentIndexCha..."
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 4), // "arg1"
QT_MOC_LITERAL(4, 49, 14), // "send_line_text"
QT_MOC_LITERAL(5, 64, 25), // "text_browser_text_changed"
QT_MOC_LITERAL(6, 90, 11), // "__log_to_ui"
QT_MOC_LITERAL(7, 102, 3), // "log"
QT_MOC_LITERAL(8, 106, 4), // "type"
QT_MOC_LITERAL(9, 111, 13), // "__log_to_file"
QT_MOC_LITERAL(10, 125, 13), // "clear_browser"
QT_MOC_LITERAL(11, 139, 13), // "on_set_pushed"
QT_MOC_LITERAL(12, 153, 13), // "pause_browser"
QT_MOC_LITERAL(13, 167, 23), // "on_checkbox_log_to_file"
QT_MOC_LITERAL(14, 191, 16), // "handle_uart_data"
QT_MOC_LITERAL(15, 208, 19), // "pushbotton_callback"
QT_MOC_LITERAL(16, 228, 2) // "id"

    },
    "uart_gui\0on_port_index_currentIndexChanged\0"
    "\0arg1\0send_line_text\0text_browser_text_changed\0"
    "__log_to_ui\0log\0type\0__log_to_file\0"
    "clear_browser\0on_set_pushed\0pause_browser\0"
    "on_checkbox_log_to_file\0handle_uart_data\0"
    "pushbotton_callback\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_uart_gui[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a /* Public */,
       4,    0,   77,    2, 0x0a /* Public */,
       5,    0,   78,    2, 0x0a /* Public */,
       6,    2,   79,    2, 0x0a /* Public */,
       6,    1,   84,    2, 0x2a /* Public | MethodCloned */,
       9,    1,   87,    2, 0x0a /* Public */,
      10,    0,   90,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x0a /* Public */,
      12,    0,   92,    2, 0x0a /* Public */,
      13,    0,   93,    2, 0x0a /* Public */,
      14,    0,   94,    2, 0x0a /* Public */,
      15,    1,   95,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,

       0        // eod
};

void uart_gui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        uart_gui *_t = static_cast<uart_gui *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_port_index_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->send_line_text(); break;
        case 2: _t->text_browser_text_changed(); break;
        case 3: _t->__log_to_ui((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->__log_to_ui((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->__log_to_file((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->clear_browser(); break;
        case 7: _t->on_set_pushed(); break;
        case 8: _t->pause_browser(); break;
        case 9: _t->on_checkbox_log_to_file(); break;
        case 10: _t->handle_uart_data(); break;
        case 11: _t->pushbotton_callback((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject uart_gui::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_uart_gui.data,
      qt_meta_data_uart_gui,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *uart_gui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *uart_gui::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_uart_gui.stringdata0))
        return static_cast<void*>(const_cast< uart_gui*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int uart_gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
