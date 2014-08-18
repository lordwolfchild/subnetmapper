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

#include "sm_ipv4editdialog.h"
#include "ui_sm_ipv4editdialog.h"
#include <QColorDialog>

sm_IPv4EditDialog::sm_IPv4EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sm_IPv4EditDialog)
{
    ui->setupUi(this);

    QString momIP=QString("192.168.0.1");
    QString momNM=QString("255.255.255.0");

    helperSubnet.setIP(momIP);
    helperSubnet.setNM(momNM);

    selectedColor.setNamedColor(tr("red"));

    ui->colorBox->setStyleSheet("QFrame { background-color: "+selectedColor.name()+" }");

}

sm_IPv4EditDialog::~sm_IPv4EditDialog()
{
    delete ui;
}

QString sm_IPv4EditDialog::getIP()
{
    return (QString::number(ui->ip_byte1->value())+QString(".")+QString::number(ui->ip_byte2->value())+QString(".")+QString::number(ui->ip_byte3->value())+QString(".")+QString::number(ui->ip_byte4->value()));
}

QString sm_IPv4EditDialog::getNM()
{
    return (QString::number(ui->nm_byte1->value())+QString(".")+QString::number(ui->nm_byte2->value())+QString(".")+QString::number(ui->nm_byte3->value())+QString(".")+QString::number(ui->nm_byte4->value()));
}

void sm_IPv4EditDialog::setIP(quint32 ip)
{
    // I'm using pointer aritmethics to calculate the ip octets...
    // get the ip into something we cannot destroy by accident. ;)
    unsigned long int ul_ip = ip;
    // Now we do the bad stuff. Get the ptr to the first byte of the ulong above.
    unsigned char* ptr = (unsigned char*)&ul_ip;
    // now use this address as an offset to select the subsequent bytes from the long.
    // Don't forget to use clean casting, or the compiler will get unhappy.
    unsigned char oct1 = *(unsigned char*)(ptr+3);
    unsigned char oct2 = *(unsigned char*)(ptr+2);
    unsigned char oct3 = *(unsigned char*)(ptr+1);
    unsigned char oct4 = *(unsigned char*)(ptr);

    // Now put it in the corresponding fields
    ui->ip_byte1->setValue(oct1);
    ui->ip_byte2->setValue(oct2);
    ui->ip_byte3->setValue(oct3);
    ui->ip_byte4->setValue(oct4);
}

void sm_IPv4EditDialog::setNM(quint32 nm)
{
    // I'm using pointer aritmethics to calculate the ip octets...
    // get the ip into something we cannot destroy by accident. ;)
    unsigned long int ul_ip = nm;
    // Now we do the bad stuff. Get the ptr to the first byte of the ulong above.
    unsigned char* ptr = (unsigned char*)&ul_ip;
    // now use this address as an offset to select the subsequent bytes from the long.
    // Don't forget to use clean casting, or the compiler will get unhappy.
    unsigned char oct1 = *(unsigned char*)(ptr+3);
    unsigned char oct2 = *(unsigned char*)(ptr+2);
    unsigned char oct3 = *(unsigned char*)(ptr+1);
    unsigned char oct4 = *(unsigned char*)(ptr);

    // Now put it in the corresponding fields
    ui->nm_byte1->setValue(oct1);
    ui->nm_byte2->setValue(oct2);
    ui->nm_byte3->setValue(oct3);
    ui->nm_byte4->setValue(oct4);
}

QString sm_IPv4EditDialog::getDescription()
{
    return ui->description->text();
}

QString sm_IPv4EditDialog::getIdentifier()
{
    return ui->identifier->text();
}

void sm_IPv4EditDialog::updateFields()
{

    QString momIP=QString::number(ui->ip_byte1->value())+QString(".")+QString::number(ui->ip_byte2->value())+QString(".")+QString::number(ui->ip_byte3->value())+QString(".")+QString::number(ui->ip_byte4->value());
    QString momNM=QString::number(ui->nm_byte1->value())+QString(".")+QString::number(ui->nm_byte2->value())+QString(".")+QString::number(ui->nm_byte3->value())+QString(".")+QString::number(ui->nm_byte4->value());

    helperSubnet.setIP(momIP);
    helperSubnet.setNM(momNM);

    ui->CIDRSuffix->setValue(helperSubnet.getCIDR());
    ui->networkSize->display((int)helperSubnet.getSize()-2);

    quint32 momwc=helperSubnet.getWildcard();
    quint32 mombc=helperSubnet.getBroadcast();
    quint32 momlip=helperSubnet.getLastUsableIP();
    quint32 momfip=helperSubnet.getFirstUsableIP();

    ui->networkAddress->setText(helperSubnet.IP2String(helperSubnet.getIP()));
    ui->wildcardAddress->setText(helperSubnet.IP2String(momwc));
    ui->broadcastAddress->setText(helperSubnet.IP2String(mombc));
    ui->firstUsableAddress->setText(helperSubnet.IP2String(momfip));
    ui->lastUsableAddress->setText(helperSubnet.IP2String(momlip));

}

void sm_IPv4EditDialog::updateCIDR()
{
    quint32 suffix=ui->CIDRSuffix->value();
    quint32 momNM=~0;

    momNM=momNM<<(32-suffix);

    helperSubnet.setNM(momNM);

    quint32 ip=helperSubnet.getIP();
    quint32 nm=helperSubnet.getNM();

    ui->ip_byte1->setValue(*((unsigned char*)(((unsigned char*)&ip)+3)));
    ui->ip_byte2->setValue(*((unsigned char*)(((unsigned char*)&ip)+2)));
    ui->ip_byte3->setValue(*((unsigned char*)(((unsigned char*)&ip)+1)));
    ui->ip_byte4->setValue(*((unsigned char*)(((unsigned char*)&ip)+0)));

    ui->nm_byte1->setValue(*((unsigned char*)(((unsigned char*)&nm)+3)));
    ui->nm_byte2->setValue(*((unsigned char*)(((unsigned char*)&nm)+2)));
    ui->nm_byte3->setValue(*((unsigned char*)(((unsigned char*)&nm)+1)));
    ui->nm_byte4->setValue(*((unsigned char*)(((unsigned char*)&nm)+0)));
}

void sm_IPv4EditDialog::colorClicked()
{
    QColorDialog colorDialog(selectedColor);
    if (colorDialog.exec()) {
        selectedColor=colorDialog.selectedColor();
        ui->colorBox->setStyleSheet("QFrame { background-color: "+selectedColor.name()+" }");
    }
};

void sm_IPv4EditDialog::setDescription(QString description)
{
    ui->description->setText(description);
}

void sm_IPv4EditDialog::setIdentifier(QString identifier)
{
    ui->identifier->setText(identifier);
}

QColor sm_IPv4EditDialog::getColor()
{
    return selectedColor;
}

void sm_IPv4EditDialog::setColor(QColor &color)
{
    selectedColor=color;
    ui->colorBox->setStyleSheet("QFrame { background-color: "+selectedColor.name()+" }");
}

