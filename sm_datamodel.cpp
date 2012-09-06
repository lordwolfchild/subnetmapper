#include "sm_datamodel.h"

SM_DataModel::SM_DataModel(QObject *parent) : QAbstractTableModel(parent)
{
}

Qt::ItemFlags SM_DataModel::flags ( const QModelIndex & index ) const
{
    switch (index.column()) {

        case 0:
            return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 1:
            return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 2:
            return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 3:
            return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        default:
            return Qt::ItemIsSelectable|Qt::ItemIsEnabled;

    };

    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QVariant SM_DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role==Qt::DisplayRole)&(orientation=Qt::Horizontal)) {
        switch(section) {
        case 0:
            return QString("Identifier");
            break;
        case 1:
            return QString("IP/CIDR");
            break;
        case 2:
            return QString("Size");
            break;
        case 3:
            return QString("Description");
            break;
        default:
            return QVariant();
        }

    } else return QVariant();
}

QVariant SM_DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
          return QVariant();
    //qDebug("Data called at (%u/%u)",index.row(),index.column());
    if (index.row()>SubnetList.count()-1) {

        switch(index.column()) {
            case 0:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetList.count())))->toStr();
                break;
            case 1:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetList.count())))->toStr();
                break;
            case 2:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetList.count())))->toStr();
                break;
            case 3:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetList.count())))->toStr();
                break;
            default:
                return QVariant();
                break;

        }

    } else {

        switch(index.column()) {
            case 0:
                return ((Subnet*)(SubnetList.at(index.row())))->getIdentifier();
                break;
            case 1:
                return ((Subnet*)(SubnetList.at(index.row())))->toStr();
                break;
            case 2:
                return ((Subnet*)(SubnetList.at(index.row())))->getSize();
                break;
            case 3:
                return ((Subnet*)(SubnetList.at(index.row())))->getDescription();
                break;
            default:
                return QVariant();
                break;

        }

    }
}

int SM_DataModel::rowCount(const QModelIndex &parent) const
{
    return SubnetList.count()+SubnetV6List.count();
}

int SM_DataModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

bool SM_DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::EditRole) {

        if (index.row()>(SubnetList.count()-1)) {

            Subnet_v6* momItem = SubnetV6List.at(index.row()-SubnetList.count());

            switch(index.column()) {

                case 0:
                    // identifier changed
                    momItem->setIdentifier((QString&)value);
                    emit dataChanged(index,index);
                    return true;
                    break;

                case 3:
                    // description changed
                    momItem->setDescription((QString&)value);
                    emit dataChanged(index,index);
                    return true;
                    break;

                default:
                    return false;
            };

        } else {

            Subnet* momItem = SubnetList.at(index.row());

            switch(index.column()) {

                case 0:
                    // identifier changed
                    momItem->setIdentifier((QString&)value);
                    emit dataChanged(index,index);
                    return true;
                    break;

                case 3:
                    // description changed
                    momItem->setDescription((QString&)value);
                    emit dataChanged(index,index);
                    return true;
                    break;

                default:
                    return false;
            };


        }

    } else return false;
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

    if (row>(SubnetList.count()-1))
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
