#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet_v6.h"
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

    Subnet_v6* momSubnet = new Subnet_v6();
    momSubnet->setIP(*mom1);
    momSubnet->setNM(*mom3);
    momSubnet->setDescription(mom2);
    momSubnet->setDescription(*mom1);

    delete mom1;
    delete mom3;
    momSubnet->dumpAll();
    delete momSubnet;


    Subnet *smom2 = new Subnet("131.220.149.6","255.255.255.224");

    smom2->dumpAll();

    delete smom2;

    return 0;
}
