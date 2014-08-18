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

#ifndef SM_IPV6EDITDIALOG_H
#define SM_IPV6EDITDIALOG_H

#include <QDialog>
#include "subnet_v6.h"
#include "ipv6validator.h"

namespace Ui {
class SM_IPv6EditDialog;
}

class SM_IPv6EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SM_IPv6EditDialog(QWidget *parent = 0);
    ~SM_IPv6EditDialog();

    QString getIP();
    QString getNM();

    void setIP(QString ip);
    void setNM(QString nm);

    QString getDescription();
    QString getIdentifier();
    void setDescription(QString description);
    void setIdentifier(QString identifier);

    QColor getColor();
    void setColor(QColor &color);

public slots:
    void updateFields();

protected slots:
    void updateCIDR();
    void colorClicked();

protected:
    Subnet_v6* helperSubnet;
    IPv6Validator validator;

private:
    Ui::SM_IPv6EditDialog *ui;
    QColor selectedColor;
};

#endif // SM_IPV6EDITDIALOG_H
