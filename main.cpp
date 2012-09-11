#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet_v6.h"
#include "subnet_v4.h"
#include <iostream>
#include <QtGlobal>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();

   Subnet *momsubnet = new Subnet_v6("1:2:3::7:8/124");

   momsubnet->dumpAll();

   delete momsubnet;

   return a.exec();

}
