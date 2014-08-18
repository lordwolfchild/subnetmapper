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

#ifndef SM_INFODOCKWIDGET_H
#define SM_INFODOCKWIDGET_H

#include <QDockWidget>
#include "subnet.h"

namespace Ui {
class SM_InfoDockWidget;
}

class SM_InfoDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SM_InfoDockWidget(Subnet *sn = 0,QWidget *parent = 0);
    ~SM_InfoDockWidget();

    void setSubnet(Subnet *sn);


public slots:

    void notesEdited();
    void updateSubnet();

private:
    Ui::SM_InfoDockWidget *ui;
    Subnet *referencedSubnet;
};

#endif // SM_INFODOCKWIDGET_H
