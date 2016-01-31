#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T11:42:18
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


PROJECT_NAME = vm

COM_INC_DIR = include
COM_SRC_DIR = src
VM_SRC_DIR = vm

INCLUDEPATH += $${COM_INC_DIR}/


TARGET =  $${PROJECT_NAME}_uart_test
TEMPLATE = app

SOURCES += $${COM_SRC_DIR}/main.cpp \
			 $${COM_SRC_DIR}/uart_handler.cpp \
			 $${COM_SRC_DIR}/uart_gui.cpp \
			$${COM_SRC_DIR}/evm.cpp \
			$${COM_SRC_DIR}/uart_gui_vm.cpp \
			$${COM_SRC_DIR}/select_bin_dg.cpp \




HEADERS  += $${COM_INC_DIR}/uart_gui.h \
			$${COM_INC_DIR}/uart_handler.h \
			$${COM_INC_DIR}/config.h \
			$${COM_INC_DIR}/type_def.h \
			$${COM_INC_DIR}/evm.h \
			$${COM_INC_DIR}/uart_gui_vm.h \
			$${COM_INC_DIR}/select_bin_dg.h \

RESOURCES += \
    resource/resource.qrc

RC_FILE = resource/1.rc

LIBS += -lWs2_32

FORMS += \
    resource/select_bin_dg.ui

