#include "subnet.h"
#include <stdio.h>

Subnet::Subnet(QObject *parent) :
    QObject(parent)
{
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

