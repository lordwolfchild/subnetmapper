#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T14:00:28
#
#-------------------------------------------------

QT       += core gui

TARGET = SubnetMapper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ttsubnetview.cpp \
    subnet.cpp \
    subnet_v6.cpp \
    sm_datamodel.cpp

HEADERS  += mainwindow.h \
    ttsubnetview.h \
    subnet.h \
    subnet_v6.h \
    sm_datamodel.h

FORMS    += mainwindow.ui
