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

    QString* mom1 = new QString("123.124.126.126");
    QString* mom3 = new QString("255.255.253.0");
    QString mom2="Back to Moinmoin!";

    Subnet* momSubnet = new Subnet();
    momSubnet->setIP(*mom1);
    momSubnet->setNM(*mom3);
    momSubnet->setDescription(mom2);
    momSubnet->setDescription(*mom1);

    delete mom1;
    delete mom3;
    momSubnet->dumpAll();

    QString momIP="121.255.123.124";
    quint32 mom=momSubnet->String2IP(momIP);
    qDebug("%s\n",qPrintable(momSubnet->IP2String(mom)));
    delete momSubnet;

    return 0;
}
