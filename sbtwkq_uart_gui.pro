#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T11:42:18
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sbtwkq_uart_gui
TEMPLATE = app


SOURCES += main.cpp\
    uart_handler.cpp \
    sbt_uart_handler.cpp \
    uart_gui.cpp

HEADERS  += \
    uart_gui.h \
    uart_handler.h \
    sbt_uart_handler.h \
    config.h \
    dev_type_chose.h \
    type_def.h \
    sbt_ui_define.h \
    sbt_type_define.h


FORMS    +=
