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
    if (index.row()>SubnetV4List.count()-1) {

        switch(index.column()) {
            case 0:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetV4List.count())))->toStr();
                break;
            case 1:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetV4List.count())))->toStr();
                break;
            case 2:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetV4List.count())))->toStr();
                break;
            case 3:
                return ((Subnet_v6*)(SubnetV6List.at(index.row()-SubnetV4List.count())))->toStr();
                break;
            default:
                return QVariant();
                break;

        }

    } else {

        switch(index.column()) {
            case 0:
                return ((Subnet_v4*)(SubnetV4List.at(index.row())))->getIdentifier();
                break;
            case 1:
                return ((Subnet_v4*)(SubnetV4List.at(index.row())))->toStr();
                break;
            case 2:
                return ((Subnet_v4*)(SubnetV4List.at(index.row())))->getSize();
                break;
            case 3:
                return ((Subnet_v4*)(SubnetV4List.at(index.row())))->getDescription();
                break;
            default:
                return QVariant();
                break;

        }

    }
}

int SM_DataModel::rowCount(const QModelIndex& /* parent */ ) const
{

    return SubnetV4List.count()+SubnetV6List.count();
}

int SM_DataModel::columnCount(const QModelIndex& /* parent */) const
{
    return 4;
}

bool SM_DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::EditRole) {

        if (index.row()>(SubnetV4List.count()-1)) {

            Subnet_v6* momItem = SubnetV6List.at(index.row()-SubnetV4List.count());

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

            Subnet_v4* momItem = SubnetV4List.at(index.row());

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

bool SM_DataModel::removeRows(int row, int count, const QModelIndex &parent)
{

    if (row>(SubnetV4List.count()-1))
    {
        Subnet_v6* mom = (Subnet_v6*)(SubnetV6List.at(row-SubnetV4List.count()));
        SubnetV6List.removeAt(row-SubnetV4List.count());
        delete mom;
        return true;
    } else {
        Subnet_v4* mom = (Subnet_v4*)(SubnetV4List.at(row));
        SubnetV4List.removeAt(row);
        delete mom;
        return true;
    };
}

void SM_DataModel::addDemos()
{
    Subnet_v4 *mom1 = new Subnet_v4("131.220.149.0","255.255.255.224","sgbit1","sgbit VLAN1");
    Subnet_v4 *mom2 = new Subnet_v4("131.220.150.0","255.255.255.224","sgbit2","sgbit VLAN2");
    Subnet_v4 *mom3 = new Subnet_v4("131.220.151.0","255.255.255.224","sgbit3","sgbit VLAN3");
    Subnet_v4 *mom4 = new Subnet_v4("131.220.152.0","255.255.255.224","sgbit4","sgbit VLAN4");

    SubnetV4List.append(mom1);
    SubnetV4List.append(mom2);
    SubnetV4List.append(mom3);
    SubnetV4List.append(mom4);

    mom1->dumpAll();
    mom2->dumpAll();
    mom3->dumpAll();
    mom4->dumpAll();

}
