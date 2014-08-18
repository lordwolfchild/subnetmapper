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

#include "ipv6validator.h"
#include "subnet_v6.h"
#include <QRegExp>

IPv6Validator::IPv6Validator(QObject *parent) :
    QValidator(parent)
{
}

void IPv6Validator::fixup(QString &input) const
{
    Subnet_v6 helperSubnet(input);
    input=helperSubnet.toString();

}

QValidator::State IPv6Validator::validate(QString &input, int & /*pos*/) const
{
    if (input.contains(QRegExp("[^0-9a-f:]{1,}",Qt::CaseInsensitive))) {
        return QValidator::Invalid;
    }

    if (input.count(':')>7) {
        return QValidator::Invalid;
    }

    if (input.contains(QRegExp("[:][:][:]{1,}",Qt::CaseInsensitive))) {
        return QValidator::Invalid;
    }


    if (input.contains(QRegExp("^([0-9a-f]{0,4}[:]){2,7}[0-9a-f]{0,4}$",Qt::CaseSensitive))) {
        if (input.contains("::")) {
            return QValidator::Acceptable;
        } else {
            if (input.count(':')==7) return QValidator::Acceptable;
        }
    }

    if (input.contains(QRegExp("^([0-9a-f]{0,4}[:]){0,8}$",Qt::CaseSensitive))) {
        return QValidator::Intermediate;
    }

    return QValidator::Intermediate;
}
