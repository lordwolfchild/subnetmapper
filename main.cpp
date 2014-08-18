/**
 *
 * This file is part of SubnetMapper.
 *
 * Copyright (C) 2008 "Thomas Thiel" <kv@wolfchild.de>
 *
 * SubnetMapper is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SubnetMapper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QApplication>
#include "mainwindow.h"
#include <stdio.h>
#include "subnet_v6.h"
#include "subnet_v4.h"
#include <iostream>
#include <QtGlobal>
#include <QSysInfo>
#include <QStyleFactory>

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
   a.setOrganizationName("wolfchild.de");
   a.setOrganizationDomain("wolfchild.de");
   a.setApplicationName("SubnetMapper");
   a.setApplicationVersion("2.1.0a");

   MainWindow w;
   w.show();

   return a.exec();
}
