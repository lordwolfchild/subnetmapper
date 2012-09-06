#include "subnet_v4.h"
#include <stdio.h>

Subnet_v4::Subnet_v4(QObject *parent) :
    Subnet(parent)
{
    _ip_address=new(quint32);
    _netmask=new(quint32);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    quint32 momIp = 1;
    quint32 momNm = ~0;
    QString mom = "n/a";

    *_ip_address=momIp;
    *_netmask=momNm;
    setDescription(mom);
    setIdentifier(mom);
    setNotes(mom);

    normalize();
}

Subnet_v4::Subnet_v4(quint32 ip, quint32 nm, QString id, QString description, QString notes, QObject *parent) :
    Subnet(parent)
{

    _ip_address=new(quint32);
    _netmask=new(quint32);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    *_ip_address=ip;
    *_netmask=nm;
    setDescription(description);
    setIdentifier(id);
    setNotes(notes);

    normalize();

}

Subnet_v4::Subnet_v4(QString ip, QString nm, QString id, QString description, QString notes, QObject *parent) :
    Subnet(parent)
{

    _ip_address=new(quint32);
    _netmask=new(quint32);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    quint32 momip = String2IP(ip);
    quint32 momnm = String2IP(nm);
    *_ip_address=momip;
    *_netmask=momnm;
    setDescription(description);
    setIdentifier(id);
    setNotes(notes);

    normalize();

}

Subnet_v4::~Subnet_v4()
{
    delete(_ip_address);
    delete(_netmask);
    delete(_description);
    delete(_notes);
    delete(_identifier);
    delete(_selected);
    delete(_color);
}

void Subnet_v4::setIP(quint32 &ip)
{
    *_ip_address=ip;
    normalize();
}

void Subnet_v4::setIP(QString &ip)
{
    *_ip_address = String2IP(ip);
    normalize();
}

void Subnet_v4::setNM(quint32 &nm)
{
    *_netmask=nm;
    normalize();
}

void Subnet_v4::setNM(QString &nm)
{
    *_netmask = String2IP(nm);
    normalize();
}

void Subnet_v4::setDescription(QString &description)
{
    *_description=description;
}

void Subnet_v4::setIdentifier(QString &identifier)
{
    *_identifier=identifier;
}

void Subnet_v4::setSelected(bool &selected)
{
        *_selected=selected;
}

void Subnet_v4::setColor(QColor &color)
{
        *_color=color;
}

void Subnet_v4::setNotes(QString &notes)
{
        *_notes=notes;
}

quint32& Subnet_v4::getIP()
{
    return *_ip_address;
}


// TODO
quint32 Subnet_v4::getLastUsableIP()
{
    if (getCIDR()<31)
            return (getBroadcast()-1);
    else
            return 0;
}

// TODO
quint32 Subnet_v4::getFirstUsableIP()
{
    if (getCIDR()<31)
            return (getIP()+1);
    else
            return 0;
}

// TODO
quint32 Subnet_v4::getWildcard()
{
    return ~(*_netmask);
}

// TODO
quint32 Subnet_v4::getBroadcast()
{
    return (getIP()|getWildcard());
}

// TODO
quint32 Subnet_v4::getSize()
{
    quint32 nm = getNM();
    quint32 wc = (~nm)+1;

    return wc;
}

// TODO
quint32 Subnet_v4::getCIDR()
{
    unsigned char cidr=32;
    unsigned long wildcard=~getNM();

    while (wildcard>0) {
            cidr--;
            wildcard>>=1;
    };

    return cidr;

}

Subnet_v4::IPversion Subnet_v4::getIPversion()
{
    return Subnet_v4::IPv4;
}

quint32& Subnet_v4::getNM()
{
    return *_netmask;
}

QString& Subnet_v4::getDescription()
{
    return *_description;
}

QString& Subnet_v4::getIdentifier()
{
    return *_identifier;
}

QString& Subnet_v4::getNotes()
{
    return *_notes;
}

bool&    Subnet_v4::getSelected()
{
    return *_selected;
}

QColor&  Subnet_v4::getColor()
{
    return *_color;
}

QString Subnet_v4::IP2String(quint32 &ip)
{
    QString outp;

    // I'm using pointer aritmethics to calculate the netmask octets...
    // get the ip into something we cannot destroy by accident. ;)
    unsigned long int ul_ip = ip;
    // Now we do the bad stuff. Get the ptr to the first byte of the ulong above.
    unsigned char* ptr = (unsigned char*)&ul_ip;
    // now use this address as an offset to select the subsequent bytes from the long.
    // Don't forget to use clean casting, or the compiler will get unhappy.
    unsigned char oct1 = *(unsigned char*)(ptr+3);
    unsigned char oct2 = *(unsigned char*)(ptr+2);
    unsigned char oct3 = *(unsigned char*)(ptr+1);
    unsigned char oct4 = *(unsigned char*)(ptr);

    // Now put it in the string which we will return afterwards.
    outp.sprintf("%u.%u.%u.%u",oct1,oct2,oct3,oct4);

    // finished!
    return outp;
}

quint32 Subnet_v4::getCIDR24Blocks()
{
    quint32 nm = getNM();

    // complement the netmask and shift it 8 bits to the right and you get the number of /24
    // nets in the Subnet_v4 (-1, because the zero counts a a net again).
    quint32 numberOfCIDRBlocks=(((~nm)>>8)+1);

    return numberOfCIDRBlocks;
}


// returns zero if string is not parseable. Attention: returns zero, if IP reads 0.0.0.0, too! :)
quint32 Subnet_v4::String2IP(QString &str_ip)
{
    // clean up the input
    QString inp_str=str_ip.trimmed();

    // check via regular expression if the string is in std. 4 byte dotted notation
    if (inp_str.contains(QRegExp("([0-9]{1,3}[.]){,3}[0-9]{1,3}")))
    {
        // Ok, the string seems to be allright. So we split.
        QStringList strlist = inp_str.split('.');

        // If there are more or less than 4 parts of the splitted string, something
        // is not right and we just reject.
        if (strlist.count()!=4) return 0;
        else
        {
            // Ok, now it seems we can do some actual conversion work. Convert the
            // single bytes to unsigned integers.
            unsigned int mom_b4 = strlist.at(0).toUInt();
            unsigned int mom_b3 = strlist.at(1).toUInt();
            unsigned int mom_b2 = strlist.at(2).toUInt();
            unsigned int mom_b1 = strlist.at(3).toUInt();

            // if one of the parsed numbers is higher than 255, it cannot be a valid IP.
            if ((mom_b4>255)|(mom_b3>255)|(mom_b2>255)|(mom_b1>255)) return 0;

            // in this variable we will save the calculated numerical ip
            quint32 mom_ip;

            // prepare soome helper pointer, so we can save the discrete bytes into
            // mom_ip conveniently and without too much casting horror
            unsigned char *b0 = ((unsigned char*)&mom_ip);
            unsigned char *b1 = ((unsigned char*)&mom_ip)+1;
            unsigned char *b2 = ((unsigned char*)&mom_ip)+2;
            unsigned char *b3 = ((unsigned char*)&mom_ip)+3;


            // save the parsed bytes into the dereferences helper pointers,
            // and therefore bytewise into the integer mom_ip.
            *b0 = *(((unsigned char*)&mom_b1)+0);
            *b1 = *(((unsigned char*)&mom_b2)+0);
            *b2 = *(((unsigned char*)&mom_b3)+0);
            *b3 = *(((unsigned char*)&mom_b4)+0);

            // return our hard earned result.
            return mom_ip;
        }
    } else return 0;
}

QString Subnet_v4::toStr()
{
    QString outp;

    outp+=IP2String(getIP());
    outp+="/";

    QString cidr;
    cidr.setNum(getCIDR());
    outp+=cidr;

    return outp;
}

// TODO
bool Subnet_v4::containsHost(quint32 &host)
{
    normalize();

    if ((host&getNM())==getIP())
         return true;
    else
         return false;
}

void Subnet_v4::normalize()
{
    // as we want to specify a Subnet_v4 here, we actually have to make sure, that the ip entered is the network address,
    // not one of the network's host addresses. Just AND IP and NM like we learned in ancient times at school.
    *_ip_address=getIP()&getNM();
}

void Subnet_v4::dumpAll()
{
     quint32 wildcard=getWildcard();
     quint32 first = getFirstUsableIP();
     quint32 last = getLastUsableIP();
     quint32 broadcast = getBroadcast();
     quint32 cidr24 = getCIDR24Blocks();

     qDebug("---DUMP START----------------------[IPv4]---");
     qDebug(" Object Address:         %p",this);
     qDebug("--------------------------------------------");
     qDebug(" Network:                %s",qPrintable(this->toStr()));
     qDebug(" Netmask:                %s",qPrintable(IP2String(getNM())));
     qDebug(" Wildcard Bits:          %s",qPrintable(IP2String(wildcard)));
     qDebug(" Size:                   %u",getSize());
     qDebug(" First Usable IP:        %s",qPrintable(IP2String(first)));
     qDebug(" Last Usable IP:         %s",qPrintable(IP2String(last)));
     qDebug(" Broadcast Address:      %s",qPrintable(IP2String(broadcast)));
     qDebug(" CIDR24 Blocks:          %u",cidr24);
     qDebug("--------------------------------------------");
     qDebug(" Identifier:");
     qDebug("   %s",qPrintable(getIdentifier()));
     qDebug(" Description:");
     qDebug("   %s",qPrintable(getDescription()));
     qDebug("--------------------------------------------");
     qDebug(" Notes dump:");
     qDebug("   %s",qPrintable(getNotes()));
     qDebug("---------------------------------DUMP END---");
}
