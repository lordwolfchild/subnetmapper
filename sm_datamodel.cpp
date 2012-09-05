#include "sm_datamodel.h"

SM_DataModel::SM_DataModel(QObject *parent) : QAbstractTableModel(parent)
{
}

Qt::ItemFlags SM_DataModel::flags ( const QModelIndex & index ) const
{
    return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QVariant SM_DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
}

QVariant SM_DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
          return QVariant();
    qDebug("Data called at (%u/%u)",index.row(),index.column());
    if (index.row()>SubnetList.count())
        return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetList.count())))->toStr();
    else
        return ((Subnet*)(SubnetList.at(index.row())))->toStr();
}

int SM_DataModel::rowCount(const QModelIndex &parent) const
{
    return SubnetList.count()+SubnetV6List.count();
}

int SM_DataModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

bool SM_DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

bool SM_DataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

bool SM_DataModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    return false;
}

bool SM_DataModel::removeRows(int row, int count, const QModelIndex &parent)
{

    if (row>SubnetList.count())
    {
        Subnet_v6* mom = (Subnet_v6*)(SubnetV6List.at(row-SubnetList.count()));
        SubnetV6List.removeAt(row-SubnetList.count());
        delete mom;
        return true;
    } else {
        Subnet* mom = (Subnet*)(SubnetList.at(row));
        SubnetList.removeAt(row);
        delete mom;
        return true;
    };
}

bool SM_DataModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    return false;
}

void SM_DataModel::addDemos()
{
    Subnet *mom1 = new Subnet("131.220.149.0","255.255.255.224","sgbit1","sgbit VLAN1");
    Subnet *mom2 = new Subnet("131.220.150.0","255.255.255.224","sgbit2","sgbit VLAN2");
    Subnet *mom3 = new Subnet("131.220.151.0","255.255.255.224","sgbit3","sgbit VLAN3");
    Subnet *mom4 = new Subnet("131.220.152.0","255.255.255.224","sgbit4","sgbit VLAN4");

    SubnetList.append(mom1);
    SubnetList.append(mom2);
    SubnetList.append(mom3);
    SubnetList.append(mom4);

    mom1->dumpAll();
    mom2->dumpAll();
    mom3->dumpAll();
    mom4->dumpAll();

}
