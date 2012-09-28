#include "sm_datamodel.h"
#include <QBrush>

SM_DataModel::SM_DataModel(QObject *parent) : QAbstractTableModel(parent)
{
}

SM_DataModel::~SM_DataModel()
{
    clearData();
}

Qt::ItemFlags SM_DataModel::flags ( const QModelIndex & index ) const
{
    switch (index.column()) {

        case 0:
            return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 1:
            return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 2:
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
            return QString("Description");
            break;
        default:
            return QVariant();
        }

    } else return QVariant();
}

QVariant SM_DataModel::data(const QModelIndex &index, int role) const
{
    if ((index.isValid())&(role==Qt::BackgroundRole)) {
        if ((index.row()%2)==1) {
            QBrush whiteBackground(Qt::white);
            return whiteBackground;
        } else {
            QBrush greyBackground(Qt::gray);
            return greyBackground;
        }
    }

    if ((index.isValid())&(role==Qt::ForegroundRole)) {
        QBrush subnetColor(((Subnet*)SubnetList.at(index.row()))->getColor());
        return subnetColor;
    }

    if ((index.isValid())&(role==Qt::UserRole)) {

        switch (index.column()) {
        case 0:
            if (((Subnet*)SubnetList.at(index.row()))->getIPversion()==Subnet::IPv4) return QString("IPv4");
            else return QString("IPv6");
        case 1:
            if (((Subnet*)SubnetList.at(index.row()))->getIPversion()==Subnet::IPv4) return ((Subnet_v4*)SubnetList.at(index.row()))->getIP();
            else {
                QPair<quint64,quint64> momip = ((Subnet_v6*)SubnetList.at(index.row()))->getIP();
                return Subnet_v6::IP2String(momip);
            };
        case 2:
            if (((Subnet*)SubnetList.at(index.row()))->getIPversion()==Subnet::IPv4) return QString("IPv4");
            else return QString("IPv6");
        default:
            return QVariant();
        }
    }

    if (!index.isValid() || role != (Qt::DisplayRole))
          return QVariant();
    else {
        switch(index.column()) {
            case 0:
                return ((Subnet*)(SubnetList.at(index.row())))->getIdentifier();
                break;
            case 1:
                return ((Subnet*)(SubnetList.at(index.row())))->toString();
                break;
            case 2:
                return ((Subnet*)(SubnetList.at(index.row())))->getDescription();
                break;
            default:
                return QVariant();
                break;

        }
    }
}

int SM_DataModel::rowCount(const QModelIndex& /* parent */ ) const
{

    return SubnetList.count();
}

int SM_DataModel::columnCount(const QModelIndex& /* parent */) const
{
    return 3;
}

bool SM_DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::EditRole) {

            Subnet* momItem = SubnetList.at(index.row());

            switch(index.column()) {

                case 0:
                    // identifier changed
                    momItem->setIdentifier((QString&)value);
                    emit dataChanged(index,index);
                    return true;
                    break;

                case 2:
                    // description changed
                    momItem->setDescription((QString&)value);
                    emit dataChanged(index,index);
                    return true;
                    break;

                default:
                    return false;
            };




    } else return false;
}

bool SM_DataModel::insertRows(int /*row*/, int /*count*/, const QModelIndex& /*parent*/)
{
    return false;
}

bool SM_DataModel::removeRows(int row, int count, const QModelIndex& /*parent*/)
{

    for (int i=0;i<count;i++) {

        Subnet* mom = (Subnet*)(SubnetList.at(row));
        SubnetList.removeAt(row);
        delete mom;
    }

    reset();

    return true;
}

void SM_DataModel::addDemos()
{

    Subnet *mom1 = new Subnet_v4("131.220.149.0","255.255.255.224","sgbit1","sgbit VLAN1");
    Subnet *mom2 = new Subnet_v4("131.220.150.0","255.255.255.0","sgbit2","sgbit VLAN2");
    Subnet *mom3 = new Subnet_v4("131.220.151.164","255.255.255.224","sgbit3","sgbit VLAN3");
    Subnet *mom4 = new Subnet_v4("131.220.152.128","255.255.255.127","sgbit4","sgbit VLAN4");

    SubnetList.append(mom1);
    SubnetList.append(mom2);
    SubnetList.append(mom3);
    SubnetList.append(mom4);

    Subnet *mom5 = new Subnet_v6("::1111:128/124","sgbit4","sgbit VLAN4");
    Subnet *mom6 = new Subnet_v6("::2222:128/124","sgbit5","sgbit VLAN5");
    Subnet *mom7 = new Subnet_v6("::3333:128/124","sgbit6","sgbit VLAN6");
    Subnet *mom8 = new Subnet_v6("1:2:3:4:5:6:7:8128/124","sgbit7","sgbit VLAN7");

    SubnetList.append(mom5);
    SubnetList.append(mom6);
    SubnetList.append(mom7);
    SubnetList.append(mom8);

}

void SM_DataModel::addSubnet(Subnet *subnet)
{
    SubnetList.append(subnet);
    insertRows(rowCount(),1);

    // I should emit dataChanged(), but the default implementation of QTableView will not update after this call, so I have to refresh all data for the views through reset().
    reset();
}

Subnet *SM_DataModel::getSubnet(int index)
{
    if ((index>=0)&(index<SubnetList.count())) return SubnetList.at(index);
    return NULL;
}


void SM_DataModel::clearData()
{
    // removes all subnets from datastorage and releases the reserved heap memory.
    int count=SubnetList.count();

    for (int i=0;i<count;i++) {
        Subnet *subnet = SubnetList.at(0);
        SubnetList.removeAt(0);
        delete subnet;
    }

    // Tell the widgets the data has changed.
    reset();

}

bool SM_DataModel::loadFromXmlStream(QXmlStreamReader &stream)
{
    // remove existing Map from memory
    clearData();
    stream.setNamespaceProcessing(false);

    bool foundMap=false;

    stream.readNext();
    while((!stream.name().toString().compare("SubnetMap"))&(!stream.atEnd())) stream.readNext();

    if (!stream.atEnd()) {
        foundMap=true;
        qDebug("XML: Found SubnetMap Tag at line %llu...",stream.lineNumber());
    }

    // parse it all!!
    while (!stream.atEnd()&foundMap) {

        stream.readNext();

        // subnet starts
        if ((stream.name()=="subnet")&(!stream.isEndElement())) {

            QXmlStreamAttributes attrs=stream.attributes();
            if (attrs.hasAttribute("ipversion")) {

                QStringRef ver = attrs.value("ipversion");

                qDebug("subnet found! (%s)",qPrintable(ver.toString()));

                Subnet *newSubnet=NULL;

                if (ver=="IPv4") newSubnet = new Subnet_v4(this);
                if (ver=="IPv6") newSubnet = new Subnet_v6(this);

                QString currentSection="";

                stream.readNext();

                while ((!stream.atEnd())&((!(stream.name()=="subnet")))) {

                    // check for Elememtn start. if yes, store the current element in currentSection.
                    if (stream.isStartElement()) {

                        qDebug("XML parser: Found %s section in line: %llu",qPrintable(stream.name().toString()),stream.lineNumber());

                        currentSection=stream.name().toString();

                    // If notan element start, parse the data into the fitting variable specified by currentSection.
                    } else if ((stream.isCharacters())&!(stream.isWhitespace())) {

                        qDebug("Data: %s (%s)",qPrintable(stream.text().toString()),qPrintable(stream.name().toString()));

                        QString momData=stream.text().toString();

                        if (currentSection=="identifier") newSubnet->setIdentifier(momData);
                        if (currentSection=="description") newSubnet->setDescription(momData);
                        if (currentSection=="notes") newSubnet->setDescription(momData);
                        if (currentSection=="color") {
                            QColor newcolor = QColor(momData);
                            newSubnet->setColor(newcolor);
                        }

                        if (ver=="IPv4") {
                            if (currentSection=="address") ((Subnet_v4*)newSubnet)->setIP(momData);
                            if (currentSection=="netmask") ((Subnet_v4*)newSubnet)->setNM(momData);
                        } else if (ver=="IPv6") {
                            if (currentSection=="address") ((Subnet_v6*)newSubnet)->setIP(momData);
                            if (currentSection=="netmask") ((Subnet_v6*)newSubnet)->setNM(momData);
                        }

                    }

                    stream.readNext();
                }

                qDebug("XML Parser: End of Subnet Element. Saving object To Model. Dumping final object data:");
                SubnetList.append(newSubnet);
                newSubnet->dumpAll();

            }

        // subnet ends
        }

    };

    reset();

    if (foundMap) return true;
    else return false;
}

bool SM_DataModel::saveToXmlStream(QXmlStreamWriter &stream)
{
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("SubnetMap");
    stream.writeAttribute("fileformat", "2");
    stream.writeAttribute("writer", "SubnetMapper");
    stream.writeAttribute("version", "2.0.0");

    for (int row = 0; row < SubnetList.count(); ++row) {

        Subnet *subnet = SubnetList.at(row);
        stream.writeStartElement("subnet");

        if (subnet->getIPversion()==Subnet::IPv4) {

            quint32 momip = ((Subnet_v4*)subnet)->getIP();
            quint32 momnm = ((Subnet_v4*)subnet)->getNM();
            QColor momcol = subnet->getColor();

            stream.writeAttribute("ipversion","IPv4");
            stream.writeTextElement("identifier",subnet->getIdentifier());
            stream.writeTextElement("address",((Subnet_v4*)subnet)->IP2String(momip));
            stream.writeTextElement("netmask",((Subnet_v4*)subnet)->IP2String(momnm));
            stream.writeTextElement("description",subnet->getDescription());
            stream.writeTextElement("notes",subnet->getNotes());
            stream.writeTextElement("color",momcol.name());

        } else {

            QPair<quint64,quint64> momip = ((Subnet_v6*)subnet)->getIP();
            QPair<quint64,quint64> momnm = ((Subnet_v6*)subnet)->getNM();
            QColor momcol = subnet->getColor();

            stream.writeAttribute("ipversion","IPv6");
            stream.writeTextElement("identifier",((Subnet_v6*)subnet)->getIdentifier());
            stream.writeTextElement("address",((Subnet_v6*)subnet)->IP2String(momip));
            stream.writeTextElement("netmask",((Subnet_v6*)subnet)->IP2String(momnm));
            stream.writeTextElement("description",((Subnet_v6*)subnet)->getDescription());
            stream.writeTextElement("notes",((Subnet_v6*)subnet)->getNotes());
            stream.writeTextElement("color",momcol.name());

        }

        stream.writeEndElement(); // Element: subnet
    }

    stream.writeEndElement(); // Element: SubnetMapper2
    stream.writeEndDocument();

    return true;
}

