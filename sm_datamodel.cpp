#include "sm_datamodel.h"
#include <QBrush>
#include <QDomDocument>
#include <QMessageBox>

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
            return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 1:
            return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        case 2:
            return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        default:
            return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;

    };

    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QVariant SM_DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role==Qt::DisplayRole)&(orientation=Qt::Horizontal)) {
        switch(section) {
        case 0:
            return QString("");
            break;
        case 1:
            return QString("Identifier");
            break;
        case 2:
            return QString("IP/CIDR");
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
    if ((index.isValid())&(role==Qt::BackgroundRole)) {

        if (index.column()==0) {
            QBrush subnetColor(((Subnet*)SubnetList.at(index.row()))->getColor());
            return subnetColor;
        };

        if ((index.row()%2)==1) {
            QBrush whiteBackground(Qt::white);
            return whiteBackground;
        } else {
            QBrush greyBackground(Qt::gray);
            return greyBackground;
        }
    }

    if ((index.isValid())&(role==Qt::ForegroundRole)) {
        QBrush blackColor(Qt::black);
        return blackColor;
    }

    if ((index.isValid())&(role==Qt::UserRole)) {

        switch (index.column()) {
        case 1:
            if (((Subnet*)SubnetList.at(index.row()))->getIPversion()==Subnet::IPv4) return QString("IPv4");
            else return QString("IPv6");
        case 2:
            if (((Subnet*)SubnetList.at(index.row()))->getIPversion()==Subnet::IPv4) return ((Subnet_v4*)SubnetList.at(index.row()))->getIP();
            else {
                QPair<quint64,quint64> momip = ((Subnet_v6*)SubnetList.at(index.row()))->getIP();
                return Subnet_v6::IP2String(momip);
            };
        case 3:
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
                return QString();
                break;
            case 1:
                return ((Subnet*)(SubnetList.at(index.row())))->getIdentifier();
                break;
            case 2:
                return ((Subnet*)(SubnetList.at(index.row())))->toString();
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

int SM_DataModel::rowCount(const QModelIndex& /* parent */ ) const
{

    return SubnetList.count();
}

int SM_DataModel::columnCount(const QModelIndex& /* parent */) const
{
    return 4;
}

bool SM_DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::EditRole) {

            Subnet* momItem = SubnetList.at(index.row());

            switch(index.column()) {

                case 1:
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

    emit(dataChanged(QModelIndex(),QModelIndex()));

    return true;
}

void SM_DataModel::addDemos()
{

    Subnet *mom0 = new Subnet_v4("192.168.1.0","255.255.254.0","sgbit0","sgbit VLAN0");
    Subnet *mom1 = new Subnet_v4("131.220.149.0","255.255.255.224","sgbit1","sgbit VLAN1");
    Subnet *mom2 = new Subnet_v4("131.220.150.0","255.255.255.0","sgbit2","sgbit VLAN2");
    Subnet *mom3 = new Subnet_v4("131.220.151.164","255.255.255.224","sgbit3","sgbit VLAN3");
    Subnet *mom4 = new Subnet_v4("131.220.152.128","255.255.255.128","sgbit4","sgbit VLAN4");

    QColor red=QColor(Qt::red);
    QColor blue=QColor(Qt::blue);
    QColor green=QColor(Qt::green);
    QColor yellow=QColor(Qt::yellow);
    QColor magenta=QColor(Qt::magenta);

    mom0->setColor(red);
    mom1->setColor(blue);
    mom2->setColor(green);
    mom3->setColor(yellow);
    mom4->setColor(magenta);

    SubnetList.append(mom0);
    SubnetList.append(mom1);
    SubnetList.append(mom2);
    SubnetList.append(mom3);
    SubnetList.append(mom4);

    Subnet *mom5 = new Subnet_v6("::1111:128/124","sgbit4","sgbit VLAN4");
    Subnet *mom6 = new Subnet_v6("::2222:128/124","sgbit5","sgbit VLAN5");
    Subnet *mom7 = new Subnet_v6("::3333:128/124","sgbit6","sgbit VLAN6");
    Subnet *mom8 = new Subnet_v6("1:2:3:4:5:6:7:8128/124","sgbit7","sgbit VLAN7");

    mom5->setColor(red);
    mom6->setColor(blue);
    mom7->setColor(green);
    mom8->setColor(yellow);


    SubnetList.append(mom5);
    SubnetList.append(mom6);
    SubnetList.append(mom7);
    SubnetList.append(mom8);

    sortData();

}

void SM_DataModel::addSubnet(Subnet *subnet)
{
    SubnetList.append(subnet);
    insertRows(rowCount(),1);

    emit(dataChanged(QModelIndex(),QModelIndex()));
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

    emit(dataChanged(QModelIndex(),QModelIndex()));

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

    emit(dataChanged(QModelIndex(),QModelIndex()));

    if (foundMap) return true;
    else return false;
}

bool SM_DataModel::loadFromDomDoc(QDomDocument &doc)
{

    QMessageBox msgBox;

    // let us check if we have a valid subnetmap

    QDomElement docElem = doc.documentElement();
    if (docElem.nodeName()=="SubnetMap") {
        if (docElem.hasAttribute("fileformat")) {
            if ((docElem.attribute("fileformat")).toInt()!=2) {
                msgBox.setText("Warning: the SubnetMap xou were trying to load has the wrong format. This Version of SubnetMapper can only read format version 2.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("The fileformat attribute of the SubnetMap node has a version number that is not equal to 2. Update SubnetMapper to the most recent version to read this file.");
                msgBox.exec();
                return false;
            };
        }
    } else return false;

    // now we know its one of ours.

    clearData();

    QDomNodeList subnetNodes = docElem.elementsByTagName("subnet");

    // qDebug("SM_DataModel::loadFromDomDoc(): found %u subnet nodes in the document.",subnetNodes.count());

    for (int i=0;i<subnetNodes.count();i++){
        QDomElement currentSubnetNode=subnetNodes.at(i).toElement();

        QDomElement addressNode     = currentSubnetNode.firstChildElement("address");
        QDomElement netmaskNode     = currentSubnetNode.firstChildElement("netmask");
        QDomElement colorNode       = currentSubnetNode.firstChildElement("color");
        QDomElement descriptionNode = currentSubnetNode.firstChildElement("description");
        QDomElement notesNode       = currentSubnetNode.firstChildElement("notes");
        QDomElement identifierNode  = currentSubnetNode.firstChildElement("identifier");

        if (currentSubnetNode.hasAttribute("ipversion")) {
            if (currentSubnetNode.attribute("ipversion")=="IPv4") {

                Subnet_v4 *newSubnet = new Subnet_v4(this);

                QString mom=netmaskNode.text();
                newSubnet->setNM(mom);

                mom=addressNode.text();
                newSubnet->setIP(mom);

                mom=identifierNode.text();
                newSubnet->setIdentifier(mom);

                mom=descriptionNode.text();
                newSubnet->setDescription(mom);

                mom=notesNode.text();
                newSubnet->setNotes(mom);

                QColor momColor= QColor(colorNode.text());
                newSubnet->setColor(momColor);

                SubnetList.append(newSubnet);


            } else if (currentSubnetNode.attribute("ipversion")=="IPv6") {

                Subnet_v6 *newSubnet = new Subnet_v6(this);

                QString mom=netmaskNode.text();
                newSubnet->setNM(mom);

                mom=addressNode.text();
                newSubnet->setIP(mom);

                mom=identifierNode.text();
                newSubnet->setIdentifier(mom);

                mom=descriptionNode.text();
                newSubnet->setDescription(mom);

                mom=notesNode.text();
                newSubnet->setNotes(mom);

                QColor momColor= QColor(colorNode.text());
                newSubnet->setColor(momColor);

                SubnetList.append(newSubnet);

            } else {

                msgBox.setText("Warning: Subnet "+QString::number(i)+" was invalid! Subnet will be skipped...");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("The Subnet was not clearly specified, the ipversion attribute is invalid."+
                                       identifierNode.text()+"\n"+
                                       addressNode.text()+"\n"+
                                       netmaskNode.text()+"\n"+
                                       descriptionNode.text()+"\n"+
                                       notesNode.text()+"\n"+
                                       colorNode.text()+"\n"
                                       );
                msgBox.exec();
            }
        } else {

            msgBox.setText("Warning: Subnet "+QString::number(i)+" was invalid! Subnet will be skipped...");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setDetailedText("The Subnet was not clearly specified, the ipversion attribute is missing."+
                                   identifierNode.text()+"\n"+
                                   addressNode.text()+"\n"+
                                   netmaskNode.text()+"\n"+
                                   descriptionNode.text()+"\n"+
                                   notesNode.text()+"\n"+
                                   colorNode.text()+"\n"
                                   );
            msgBox.exec();
        }

    }

    sortData();

    emit(dataChanged(QModelIndex(),QModelIndex()));

    return true;
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

bool SM_DataModel::SubnetLessThan(const Subnet* s1, const Subnet* s2)
{
    Subnet* sv1=(Subnet*)s1;
    Subnet* sv2=(Subnet*)s2;

    return sv1->isLessThan(sv2);
}

void SM_DataModel::sortData()
{
    qStableSort(SubnetList.begin(), SubnetList.end(), SM_DataModel::SubnetLessThan);
}

void SM_DataModel::dumpAllSubnets()
{
    qDebug("---vv START vv----------------------------------------------------> dumpAllSubnets() <---");
    for (int i=0;i<SubnetList.count();i++) qDebug("%s",qPrintable(SubnetList.at(i)->toString()));
    qDebug("---^^  END  ^^----------------------------------------------------> dumpAllSubnets() <---");
};




