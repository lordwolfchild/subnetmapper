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

#ifndef SM_DATAMODEL_H
#define SM_DATAMODEL_H

#include <QAbstractTableModel>
#include "subnet_v4.h"
#include "subnet_v6.h"
#include "subnet.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QDomDocument>


class SM_DataModel : public QAbstractTableModel
{
public:
    SM_DataModel(QObject *parent = 0);
    ~SM_DataModel();

    // mandatory read-only functions for a model subclass:
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

    // mandatory functions for editable model subclasses:
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    // mandatory functions for resizable model subclasses:
    bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );

    void addSubnet(Subnet *subnet);

    void reset();

    Subnet* getSubnet(int index);

    void clearData();
    bool loadFromXmlStream (QXmlStreamReader &stream);
    bool loadFromDomDoc (QDomDocument &doc);

    bool saveToXmlStream(QXmlStreamWriter &stream);

    static bool SubnetLessThan(const Subnet* s1, const Subnet* s2);
    void sortData();

    void dumpAllSubnets();

protected:

    QList<Subnet*>  SubnetList;

};

#endif // SM_DATAMODEL_H
