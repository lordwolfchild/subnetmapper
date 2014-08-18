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
 * along with SubnetMapper.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SM_IPV4EDITDIALOG_H
#define SM_IPV4EDITDIALOG_H

#include <QDialog>
#include "subnet_v4.h"

namespace Ui {
class sm_IPv4EditDialog;
}

class sm_IPv4EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sm_IPv4EditDialog(QWidget *parent = 0);
    ~sm_IPv4EditDialog();

    QString getIP();
    QString getNM();

    void setIP(quint32 ip);
    void setNM(quint32 nm);

    QString getDescription();
    QString getIdentifier();
    void setDescription(QString description);
    void setIdentifier(QString identifier);
    QColor getColor();
    void setColor(QColor &color);

protected slots:
    void updateFields();
    void updateCIDR();
    void colorClicked();

protected:
    Subnet_v4 helperSubnet;

private:
    Ui::sm_IPv4EditDialog *ui;
    QColor selectedColor;
};

#endif // SM_IPV4EDITDIALOG_H
