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

#ifndef SM_MODELBACKEND_H
#define SM_MODELBACKEND_H

#include <QObject>
#include "subnet_v4.h"
#include "subnet_v6.h"
#include "subnet.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QDomDocument>

class SM_ModelBackend : public QObject
{
    Q_OBJECT
public:
    explicit SM_ModelBackend(QObject *parent = 0);
    ~SM_ModelBackend();

    Subnet* getSubnet(int index);
    void addSubnet(Subnet *subnet);
    void clearData();

    int count();
    int count6();
    int count4();

    bool loadFromXmlStream (QXmlStreamReader &stream);
    bool loadFromDomDoc (QDomDocument &doc);

    bool saveToXmlStream(QXmlStreamWriter &stream);

    static bool SubnetLessThan(const Subnet* s1, const Subnet* s2);
    void sortData();

    void dumpAllSubnets();

signals:

public slots:

private:

    QList<Subnet*>  SubnetList;
    QList<Subnet*>  Subnet4List;
    QList<Subnet*>  Subnet6List;


};

#endif // SM_MODELBACKEND_H
