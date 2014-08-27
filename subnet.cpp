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

void Subnet::setColor(QColor color)
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

