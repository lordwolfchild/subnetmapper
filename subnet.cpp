#include "subnet.h"
#include <stdio.h>
#include "subnet_v4.h"
#include "subnet_v6.h"

Subnet::Subnet(QObject *parent) :
    QObject(parent)
{
}

QColor Subnet::getColor()
{
    return *_color;
}

void Subnet::setColor(QColor &color)
{
    *_color=color;
}

bool Subnet::isV4()
{
    if (getIPversion()==Subnet::IPv4) return true;
    return false;
}

bool Subnet::isV6()
{
    if (getIPversion()==Subnet::IPv6) return true;
    return false;
}

