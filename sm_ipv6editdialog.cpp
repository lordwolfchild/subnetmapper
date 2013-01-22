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
    updateFields();
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
    ui->address->setText(ip);
}

void SM_IPv6EditDialog::setNM(QString nm)
{
    ui->netmask->setText(nm);
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

void SM_IPv6EditDialog::setColor(QColor &color)
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
    QPair<quint64,quint64> mombc=helperSubnet->getBroadcast();
    QPair<quint64,quint64> momlip=helperSubnet->getLastUsableIP();
    QPair<quint64,quint64> momfip=helperSubnet->getFirstUsableIP();

    ui->networkAddress->setText(helperSubnet->toString());
    ui->wildcardAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momwc)));
    ui->broadcastAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(mombc)));
    ui->firstUsableAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momfip)));
    ui->lastUsableAddress->setText(helperSubnet->reduceIP(helperSubnet->IP2String(momlip)));

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
