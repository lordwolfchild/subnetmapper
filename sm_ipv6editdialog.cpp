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

#include "sm_ipv6editdialog.h"
#include "ui_sm_ipv6editdialog.h"
#include "ipv6validator.h"
#include <QColorDialog>

SM_IPv6EditDialog::SM_IPv6EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SM_IPv6EditDialog)
{
    helperSubnet = new Subnet_v6(this);
    ui->setupUi(this);
    ui->address->setValidator(&validator);
    ui->netmask->setValidator(&validator);
}

SM_IPv6EditDialog::~SM_IPv6EditDialog()
{
    delete ui;
    delete helperSubnet;
}

QString SM_IPv6EditDialog::getIP()
{
    return ui->address->text();
}

QString SM_IPv6EditDialog::getNM()
{
    return ui->netmask->text();
}

void SM_IPv6EditDialog::setIP(QString ip)
{
    //ui->address->setText(ip);
    ui->address->setText(helperSubnet->reduceIP(ip));
}

void SM_IPv6EditDialog::setNM(QString nm)
{
    //ui->netmask->setText(nm);
    ui->netmask->setText(helperSubnet->reduceIP(nm));
}

QString SM_IPv6EditDialog::getDescription()
{
    return ui->description->text();
}

QString SM_IPv6EditDialog::getIdentifier()
{
    return ui->identifier->text();
}

void SM_IPv6EditDialog::setDescription(QString description)
{
    ui->description->setText(description);
}

void SM_IPv6EditDialog::setIdentifier(QString identifier)
{
    ui->identifier->setText(identifier);
}

QColor SM_IPv6EditDialog::getColor()
{
    return selectedColor;
}

void SM_IPv6EditDialog::setColor(QColor color)
{
    selectedColor=color;
    ui->colorBox->setStyleSheet("QFrame { background-color: "+selectedColor.name()+" }");
}

void SM_IPv6EditDialog::updateFields()
{
    QString momip=ui->address->text();
    QString momnm=ui->netmask->text();

    helperSubnet->setIP(momip);
    helperSubnet->setNM(momnm);

    ui->CIDRSuffix->setValue(helperSubnet->getCIDR());

    QPair<quint64,quint64> momwc=helperSubnet->getWildcard();
    QPair<quint64,quint64> momipnum=helperSubnet->getIP();
    QPair<quint64,quint64> momnmnum=helperSubnet->getNM();
    QPair<quint64,quint64> mombc=helperSubnet->getBroadcast();
    QPair<quint64,quint64> momlip=helperSubnet->getLastUsableIP();
    QPair<quint64,quint64> momfip=helperSubnet->getFirstUsableIP();

    ui->networkAddress->setText(helperSubnet->toString());
    ui->wildcardAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momwc)));
    ui->broadcastAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(mombc)));
    ui->firstUsableAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momfip)));
    ui->lastUsableAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momlip)));

    if (sender()==ui->address) ui->address->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momipnum)));
    if (sender()==ui->netmask) ui->netmask->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momnmnum)));

}

void SM_IPv6EditDialog::updateCIDR()
{
    QString momip=ui->address->text();

    helperSubnet->setNM(ui->CIDRSuffix->value());
    helperSubnet->setIP(momip);

    QPair<quint64,quint64> momwc=helperSubnet->getWildcard();
    QPair<quint64,quint64> mombc=helperSubnet->getBroadcast();
    QPair<quint64,quint64> momlip=helperSubnet->getLastUsableIP();
    QPair<quint64,quint64> momfip=helperSubnet->getFirstUsableIP();
    QPair<quint64,quint64> momnm=helperSubnet->getNM();

    ui->netmask->setText(helperSubnet->reduceIP((helperSubnet->IP2String(momnm))));
    ui->networkAddress->setText(helperSubnet->toString());
    ui->wildcardAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momwc)));
    ui->broadcastAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(mombc)));
    ui->firstUsableAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momfip)));
    ui->lastUsableAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momlip)));

}

void SM_IPv6EditDialog::colorClicked()
{
    QColorDialog colorDialog(selectedColor);
    if (colorDialog.exec()) {
        selectedColor=colorDialog.selectedColor();
        ui->colorBox->setStyleSheet("QFrame { background-color: "+selectedColor.name()+" }");
    }
}
