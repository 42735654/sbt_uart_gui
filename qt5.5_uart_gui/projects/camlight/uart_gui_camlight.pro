#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T11:42:18
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


PROJECT_NAME = camlight
DEFINES += DEV_CAMLIGHT

PROJECT_INC_DIR = ../../include/$${PROJECT_NAME}
PROJECT_SRC_DIR = ../../src/$${PROJECT_NAME}

COM_INC_DIR = ../../include
COM_SRC_DIR = ../../src
INCLUDEPATH += $${COM_INC_DIR}/
INCLUDEPATH += $${PROJECT_INC_DIR}/

TARGET =$${PROJECT_NAME}_uart_test
TEMPLATE = app

SOURCES += $${COM_SRC_DIR}/main.cpp \
			 $${COM_SRC_DIR}/uart_handler.cpp \
			 $${COM_SRC_DIR}/uart_gui.cpp \

SOURCES += $${PROJECT_SRC_DIR}/$${PROJECT_NAME}_uart_handler.cpp \


HEADERS  += $${COM_INC_DIR}/uart_gui.h \
			$${COM_INC_DIR}/uart_handler.h \
			$${COM_INC_DIR}/config.h \
			$${COM_INC_DIR}/type_def.h \

HEADERS += $${PROJECT_INC_DIR}/$${PROJECT_NAME}_uart_handler.h \
			$${PROJECT_INC_DIR}/$${PROJECT_NAME}_ui_define.h \
			$${PROJECT_INC_DIR}/$${PROJECT_NAME}_type_define.h \
			$${PROJECT_INC_DIR}/$${PROJECT_NAME}_cmd_define.h \

RESOURCES += \
    ../../resource/resource.qrc

RC_FILE = ../../resource/1.rc


