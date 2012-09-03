#include "subnet.h"
#include <stdio.h>

Subnet::Subnet(QObject *parent) :
    QObject(parent)
{
    _ip_address=new(quint32);
    _netmask=new(quint32);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    quint32 momIp = 1;
    quint32 momNm = ~0;
    QString mom = "n/a";

    setIP(momIp);
    setNM(momNm);
    normalize();
    setDescription(mom);
    setIdentifier(mom);
    setNotes(mom);
}

Subnet::~Subnet()
{
    delete(_ip_address);
    delete(_netmask);
    delete(_description);
    delete(_notes);
    delete(_identifier);
    delete(_selected);
    delete(_color);
}

void Subnet::setIP(quint32 &ip)
{
    *_ip_address=ip;
}

void Subnet::setNM(quint32 &nm)
{
    *_netmask=nm;
}

void Subnet::setDescription(QString &description)
{
    *_description=description;
    printf("%p\n",(void*)_description);
}

void Subnet::setIdentifier(QString &identifier)
{
    *_identifier=identifier;
}

void Subnet::setSelected(bool &selected)
{
        *_selected=selected;
}

void Subnet::setColor(QColor &color)
{
        *_color=color;
}

void Subnet::setNotes(QString &notes)
{
        *_notes=notes;
}

quint32& Subnet::getIP()
{
    return *_ip_address;
}

quint32 Subnet::getLastUsableIP()
{
    return *_ip_address;
}

quint32 Subnet::getFirstUsableIP()
{
    return *_ip_address;
}

quint32 Subnet::getWildcard()
{
    return *_ip_address;
}

quint32 Subnet::getBroadcast()
{
    return *_ip_address;
}

quint32 Subnet::getSize()
{
    return *_ip_address;
}

quint32 Subnet::getCIDR()
{
    return *_ip_address;
}

quint32& Subnet::getNM()
{
    return *_netmask;
}

QString& Subnet::getDescription()
{
    return *_description;
}

QString& Subnet::getIdentifier()
{
    return *_identifier;
}

QString& Subnet::getNotes()
{
    return *_notes;
}

bool&    Subnet::getSelected()
{
    return *_selected;
}

QColor&  Subnet::getColor()
{
    return *_color;
}

QString Subnet::IP2String(quint32 &ip)
{
    ip++;
    return QString("Hey.");
}

QString Subnet::toString()
{
    QString outp="Jo.";
    return outp;
}

bool   Subnet::containsHost(quint32 &host)
{
    host++;
    return false;
}

void Subnet::normalize()
{
}

QString dumpAll()
{
    QString outp="Jo.";
    return outp;
}

