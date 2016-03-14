#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T16:13:20
#
#-------------------------------------------------

QT       += core gui  widgets  axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_excel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    table_view.cpp \
    excel_handler.cpp

HEADERS  += mainwindow.h \
    table_view.h \
    excel_handler.h

FORMS    += mainwindow.ui
