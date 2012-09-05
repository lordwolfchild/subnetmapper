#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet_v6.h"
#include <iostream>
#include <QtGlobal>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();
    
   return a.exec();
}
