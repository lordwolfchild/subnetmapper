#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T14:00:28
#
#-------------------------------------------------

QT       += core gui xml network svg

TARGET = SubnetMapper
TEMPLATE = app

INCLUDEPATH += /usr/include/python2.7 \
    /usr/include/PythonQt

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
    sm_aboutdialog.cpp

HEADERS  += mainwindow.h \
    subnet.h \
    subnet_v6.h \
    sm_datamodel.h \
    subnet_v4.h \
    sm_ipv4editdialog.h \
    sm_ipv6editdialog.h \
    ipv6validator.h \
    sm_subnetwidget.h \
    sm_aboutdialog.h

FORMS    += \
    sm_ipv4editdialog.ui \
    sm_ipv6editdialog.ui \
    sm_aboutdialog.ui

RESOURCES += \
    sm_res.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lPythonQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lPythonQt
else:symbian: LIBS += -lPythonQt -lpython2.7
else:unix: LIBS += -L/usr/lib/ -lPythonQt -lpython2.7

DEPENDPATH += /usr/include
