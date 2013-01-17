#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet_v6.h"
#include "subnet_v4.h"
#include <iostream>
#include <QtGlobal>
#include <QSysInfo>

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
   a.setOrganizationName("wolfchild.de");
   a.setOrganizationDomain("wolfchild.de");
   a.setApplicationName("SubnetMapper");
   a.setApplicationVersion("2.0.86");

   MainWindow w;
   w.show();

   return a.exec();
}
