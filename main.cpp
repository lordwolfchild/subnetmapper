#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet.h"
#include <iostream>
#include <QtGlobal>

int main(int argc, char *argv[])
{
 //   QApplication a(argc, argv);
 //   MainWindow w;
 //   w.show();
    
 //   return a.exec();

    printf ("Hello world.\n");

    QString* mom1 = new QString("Test");
    QString mom2="Back to Moinmoin!";

    Subnet* momSubnet = new Subnet();
    momSubnet->setDescription(mom2);
    momSubnet->setDescription(*mom1);

    delete mom1;
    printf("%s\n",qPrintable(momSubnet->getDescription()));
    delete momSubnet;

    return 0;
}
