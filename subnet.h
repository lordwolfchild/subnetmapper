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

#ifndef SUBNET_H
#define SUBNET_H

#include <QObject>
#include <QColor>
#include <QString>

// Mainly taken from SubnetMapper Project that was developed under cocoa. May need some adjustments now.


// IPv4 Base subnet Class Definition
class Subnet : public QObject
{
    Q_OBJECT
public:
    explicit Subnet(QObject *parent = 0);

    virtual void setDescription(QString &description) = 0;
    virtual void setIdentifier(QString &identifier) = 0;
    virtual void setSelected(bool selected) = 0;
    virtual void setNotes(QString &notes) = 0;

    virtual QString getStrNM() = 0;
    virtual QString getStrWC() = 0;

    virtual QString& getDescription() = 0;
    virtual QString& getIdentifier() = 0;
    virtual QString& getNotes() = 0;

    QColor getColor();
    void setColor(QColor &color);

    virtual void dumpAll() = 0;
    virtual QString toString() = 0;

    enum IPversion { IPv4, IPv6 };

    virtual IPversion getIPversion() = 0;

    virtual bool isLessThan(Subnet* subnet) = 0;

    bool isV4();
    bool isV6();

protected:

    QColor *_color;
};

#endif // SUBNET_H
