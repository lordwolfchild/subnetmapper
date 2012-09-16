#ifndef SM_DATAMODEL_H
#define SM_DATAMODEL_H

#include <QAbstractTableModel>
#include "subnet_v4.h"
#include "subnet_v6.h"
#include "subnet.h"
#include <QXmlStreamWriter>

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

    void addDemos();
    void addSubnet(Subnet *subnet);

    void clearData();
    bool loadFromXmlStream (QXmlStreamReader &stream);

    bool saveToXmlStream(QXmlStreamWriter &stream);

    void sortByColumn(int col);

protected:

    QList<Subnet*>  SubnetList;

};

#endif // SM_DATAMODEL_H
