#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T14:00:28
#
#-------------------------------------------------

QT       += core gui xml network svg

TARGET = SubnetMapper
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    subnet.cpp \
    subnet_v6.cpp \
    sm_datamodel.cpp \
    subnet_v4.cpp \
    sm_ipv4editdialog.cpp \
    sm_ipv6editdialog.cpp \
    ipv6validator.cpp \
    sm_subnetwidget.cpp \
    sm_aboutdialog.cpp \
    sm_infodockwidget.cpp \
    sm_configdialog.cpp

HEADERS  += mainwindow.h \
    subnet.h \
    subnet_v6.h \
    sm_datamodel.h \
    subnet_v4.h \
    sm_ipv4editdialog.h \
    sm_ipv6editdialog.h \
    ipv6validator.h \
    sm_subnetwidget.h \
    sm_aboutdialog.h \
    sm_infodockwidget.h \
    sm_configdialog.h \
    generated_version.h

FORMS    += \
    sm_ipv4editdialog.ui \
    sm_ipv6editdialog.ui \
    sm_aboutdialog.ui \
    sm_infodockwidget.ui \
    sm_configdialog.ui

RESOURCES += \
    sm_res.qrc
