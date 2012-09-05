#ifndef SM_DATAMODEL_H
#define SM_DATAMODEL_H

#include <QAbstractTableModel>
#include "subnet.h"
#include "subnet_v6.h"

class SM_DataModel : public QAbstractTableModel
{
public:
    SM_DataModel(QObject *parent = 0);

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
    bool insertColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    bool removeColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );

    void addDemos();

protected:

    QList<Subnet*>     SubnetList;
    QList<Subnet_v6*>  SubnetV6List;

};

#endif // SM_DATAMODEL_H
