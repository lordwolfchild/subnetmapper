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

#include "sm_modelbackend.h"

#include <QBrush>
#include <QDomDocument>
#include <QMessageBox>
#include <QApplication>

SM_ModelBackend::SM_ModelBackend(QObject *parent) :
    QObject(parent)
{

}

SM_ModelBackend::~SM_ModelBackend()
{
    clearData();
}

Subnet* SM_ModelBackend::getSubnet(int index)
{
    if ((index>=0)&(index<SubnetList.count())) return SubnetList.at(index);
    return NULL;

}

Subnet_v4 *SM_ModelBackend::getSubnet4(int index)
{
    if ((index>=0)&(index<Subnet4List.count())) return (Subnet_v4*)(Subnet4List.at(index));
    return NULL;

}

Subnet_v6* SM_ModelBackend::getSubnet6(int index)
{
    if ((index>=0)&(index<Subnet6List.count())) return (Subnet_v6*)(Subnet6List.at(index));
    return NULL;

}

// converts global index to specific index, returns -1 if unsuccessful (should never be the case!!)
int SM_ModelBackend::index6toAll(int index)
{
    Subnet* subnet=getSubnet6(index);
    return SubnetList.indexOf(subnet);
}

// converts global index to specific index, returns -1 if unsuccessful (should never be the case!!)
int SM_ModelBackend::index4toAll(int index)
{
    Subnet* subnet=getSubnet4(index);
    return SubnetList.indexOf(subnet);
}

// converts ipv6 index to global index, returns -1 if unsuccessful (should never be the case!!)
int SM_ModelBackend::indexAllto6(int index)
{
    Subnet* subnet=getSubnet(index);
    return Subnet6List.indexOf(subnet);
}

// converts ipv4 index to global index, returns -1 if unsuccessful (should never be the case!!)
int SM_ModelBackend::indexAllto4(int index)
{
    Subnet* subnet=getSubnet(index);
    return Subnet4List.indexOf(subnet);
}

// returns selected Index or subnetList.count+1
int SM_ModelBackend::getSelectedIndex()
{
    bool selectedSubnetFound=false;
    int selectedSubnetIndex=0;

    for (int i=0;i<SubnetList.count();i++) {

        // && imperative!! otherwise the right operand will be checked anyways and will produce
        // a SEGV, because it simply is no (Subnet_v4*) as checked for on the left side...
        if ((SubnetList.at(i)->isV4())&&(((Subnet_v4*)SubnetList.at(i))->getSelected())) {
            selectedSubnetFound=true;
            selectedSubnetIndex=i;
        };

        // && imperative!! otherwise the right operand will be checked anyways and will produce
        // a SEGV, because it simply is no (Subnet_v4*) as checked for on the left side...
        if ((SubnetList.at(i)->isV6())&&(((Subnet_v6*)SubnetList.at(i))->getSelected())) {
            selectedSubnetFound=true;
            selectedSubnetIndex=i;
        };
    };


    if (selectedSubnetFound) return selectedSubnetIndex;
    else return SubnetList.count()+1;
}


void SM_ModelBackend::removeSubnet(Subnet *subnet)
{
    QMessageBox msgBox;

    if (subnet==NULL) return;
    else {

        if (subnet->isV4()) {
            int numberOfDeletions=SubnetList.removeAll(subnet)+Subnet4List.removeAll(subnet);
            delete subnet;
            emit data4Changed();
            emit dataChanged();

            if (numberOfDeletions<2)
            {
                msgBox.setText("Warning: Deletion failed, no reference to this Subnet found.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("You tried to delete a subnet (V4) which was not found in the data backend. This should not happen and is most probably a bad bug in this software.");
                msgBox.exec();
                qDebug("SM_ModelBAckend::removeSubnet(Subnet*): deletion (V4) failed, less than two references were removed from list.");
            };

            if (numberOfDeletions>2)
            {
                msgBox.setText("Warning: Deletion failed, too many references to this Subnet found.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("You tried to delete a subnet (V6) which was stored multiple times in the data backend. This should not happen and is most probably a bad bug in this software.");
                msgBox.exec();
                qDebug("SM_ModelBAckend::removeSubnet(Subnet*): deletion (V6) failed, more than two references were removed from list.");
            };


        } else {

            int numberOfDeletions=SubnetList.removeAll(subnet)+Subnet6List.removeAll(subnet);
            delete subnet;
            emit data6Changed();
            emit dataChanged();

            if (numberOfDeletions<2)
            {
                msgBox.setText("Warning: Deletion failed, no reference to this Subnet found.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("You tried to delete a subnet which was not found in the data backend. This should not happen and is most probably a bad bug in this software. It would be wise not to save this file.");
                msgBox.exec();
                qDebug("SM_ModelBAckend::removeSubnet(Subnet*): deletion failed, less than two references were removed from list.");
            };

            if (numberOfDeletions>2)
            {
                msgBox.setText("Warning: Deletion failed, too many references to this Subnet found.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("You tried to delete a subnet which was stored multiple times in the data backend. This should not happen and is most probably a bad bug in this software. It would be wise not to save this file.");
                msgBox.exec();
                qDebug("SM_ModelBAckend::removeSubnet(Subnet*): deletion failed, more than two references were removed from list.");
            };

        }

    };

}

void SM_ModelBackend::removeSubnet(int index)
{

    removeSubnet(SubnetList.at(index));

}

void SM_ModelBackend::addSubnet(Subnet *subnet)
{
    SubnetList.append(subnet);
    emit dataChanged();

    if (subnet->isV4()) {
        Subnet4List.append(subnet);
        emit data4Changed();
    } else {
        Subnet6List.append(subnet);
        emit data6Changed();
    };

    sortData();
}

void SM_ModelBackend::changedData()
{
    qDebug("SM_ModelBackend: emitting dataChanged()");
    emit dataChanged();
}

void SM_ModelBackend::changedData4()
{
    qDebug("SM_ModelBackend: emitting data4Changed()");
    emit data4Changed();
}

void SM_ModelBackend::changedData6()
{
    qDebug("SM_ModelBackend: emitting data6Changed()");
    emit data6Changed();
}

void SM_ModelBackend::clearData(bool noEmit)
{

    // removes all subnets from datastorage and releases the reserved heap memory.
    int count=SubnetList.count();

    for (int i=0;i<count;i++) {
        Subnet *subnet = SubnetList.at(0);
        SubnetList.removeAt(0);
        delete subnet;
    }

    // As SubnetList already contained all pointers to our subnets,
    // we can safely kill the two specialized lists without leaving leaks.
    Subnet4List.clear();
    Subnet6List.clear();

    if (!noEmit) {
        emit dataChanged();
        emit data4Changed();
        emit data6Changed();
        emit modelEmptied();
    };

}

int SM_ModelBackend::count()
{
    return SubnetList.count();
}

int SM_ModelBackend::count6()
{
    return Subnet6List.count();
}

int SM_ModelBackend::count4()
{
    return Subnet4List.count();
}

bool SM_ModelBackend::loadFromDomDoc (QDomDocument &doc)
{

    QMessageBox msgBox;

    // let us check if we have a valid subnetmap

    QDomElement docElem = doc.documentElement();
    if (docElem.nodeName()=="SubnetMap") {
        if (docElem.hasAttribute("fileformat")) {
            if ((docElem.attribute("fileformat")).toInt()!=2) {
                msgBox.setText("Warning: the SubnetMap you were trying to load has the wrong format. This Version of SubnetMapper can only read format version 2.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setDetailedText("The fileformat attribute of the SubnetMap node has a version number that is not equal to 2. Update SubnetMapper to the most recent version to read this file.");
                msgBox.exec();
                return false;
            };
        }
    } else return false;

    // now we know its one of ours.

    // we can clear all data now, but we do emit only the message to notify
    // everyone about the emptied model. The change-signals wil be emitted
    // at the end of this function. This makes sure that no widget will work
    // without a valid reason at this point.
    clearData(true);
    emit modelEmptied();

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
                Subnet4List.append(newSubnet);


            } else if (currentSubnetNode.attribute("ipversion")=="IPv6") {

                Subnet_v6 *newSubnet = new Subnet_v6(this);

                QString mom=netmaskNode.text();
                //qDebug("SM_ModelBackend::loadFromDomDoc(): parsed IPv6 Netmask: %s",qPrintable(mom.toUtf8()));
                newSubnet->setNM(mom);

                mom=addressNode.text();
                //qDebug("SM_ModelBackend::loadFromDomDoc(): parsed IPv6 IP: %s",qPrintable(mom.toUtf8()));
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
                Subnet6List.append(newSubnet);

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

    emit data4Changed();
    emit data6Changed();
    emit dataChanged();

    // uncomment for debugging purposes, prints all objects in model
    dumpAllSubnets();

    return true;

}

bool SM_ModelBackend::saveToXmlStream(QXmlStreamWriter &stream)
{
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("SubnetMap");
    stream.writeAttribute("fileformat", "2");
    stream.writeAttribute("writer", "SubnetMapper");
    stream.writeAttribute("version", qApp->applicationVersion().toUtf8());

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

bool SM_ModelBackend::SubnetLessThan(const Subnet* s1, const Subnet* s2)
{

    Subnet* sv1=(Subnet*)s1;
    Subnet* sv2=(Subnet*)s2;

    return sv1->isLessThan(sv2);

}

void SM_ModelBackend::sortData()
{

    qStableSort(SubnetList.begin(), SubnetList.end(), SM_ModelBackend::SubnetLessThan);
    qStableSort(Subnet4List.begin(), Subnet4List.end(), SM_ModelBackend::SubnetLessThan);
    qStableSort(Subnet6List.begin(), Subnet6List.end(), SM_ModelBackend::SubnetLessThan);

}

void SM_ModelBackend::dumpAllSubnets()
{

    qDebug("--- START  | all vv-----------------------------------------------> dumpAllSubnets() <---");
    for (int i=0;i<SubnetList.count();i++) qDebug("%s",qPrintable(SubnetList.at(i)->toString()));
    qDebug("--- all ^^ | IPv4 vv----------------------------------------------> dumpAllSubnets() <---");
    for (int i=0;i<Subnet4List.count();i++) qDebug("%s",qPrintable(Subnet4List.at(i)->toString()));
    qDebug("---IPv4 ^^ | IPv6 vv----------------------------------------------> dumpAllSubnets() <---");
    for (int i=0;i<Subnet6List.count();i++) qDebug("%s",qPrintable(Subnet6List.at(i)->toString()));
    qDebug("---IPv6 ^^ | END  ------------------------------------------------> dumpAllSubnets() <---");

}

// sets subnet with index as selected. returns false if no valid selection possible.
bool SM_ModelBackend::selectIndex(int index)
{
    int currentSelection=getSelectedIndex();

    if ((currentSelection!=index)&(currentSelection<SubnetList.count())) {
        getSubnet(getSelectedIndex())->setSelected(false);
        emit dataChanged();
    };

    if (index<SubnetList.count()) {
        getSubnet(index)->setSelected(true);
        emit dataChanged();
    } else {
        return false;
    };

    return true;
}
