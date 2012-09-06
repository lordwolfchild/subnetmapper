#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet_v6.h"
#include <iostream>
#include <QtGlobal>

int main(int argc, char *argv[])
{
/*   QApplication a(argc, argv);
   MainWindow w;
   w.show();

   return a.exec();*/

    Subnet_v6 momsubnet;

    momsubnet.dumpAll();

    QString Test = "      2001:db8:a3::8a2e:0370:44   ";
    QPair<quint64,quint64> momip = qMakePair(~(quint64)0,~(quint64)0);

    qDebug("-%s-\n",qPrintable(momsubnet.normalizeIP(Test)));

    return 0;
}
