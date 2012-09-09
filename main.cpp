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

    Subnet_v6 momsubnet("131:23:85::8811","ffff:ffff:ffff:ffff:0000:0000:0000:0000");

    momsubnet.dumpAll();

    QString Test = "      131:23:85:8811   ";
    QPair<quint64,quint64> momip = qMakePair(~(quint64)0,~(quint64)0);

    qDebug("-%s- Original",qPrintable(Test));
    qDebug("-%s-\n",qPrintable(momsubnet.normalizeIP(Test)));

    return 0;
}
