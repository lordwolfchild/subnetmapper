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
    _color=new QColor("lightgrey");

    quint32 momIp = 1;
    quint32 momNm = ~0;
    QString mom = "n/a";

    *_ip_address=momIp;
    *_netmask=momNm;
    *_selected=false;
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
    _color=new QColor("lightgrey");

    *_ip_address=ip;
    *_netmask=nm;
    *_selected=false;
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
    _color=new QColor("lightgrey");

    quint32 momip = String2IP(ip);
    quint32 momnm = String2IP(nm);
    *_ip_address=momip;
    *_netmask=momnm;
    *_selected=false;
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

void Subnet_v4::setSelected(bool selected)
{
        *_selected=selected;
}

void Subnet_v4::setNotes(QString &notes)
{
        *_notes=notes;
}

quint32& Subnet_v4::getIP()
{
    return *_ip_address;
}


quint32 Subnet_v4::getLastUsableIP()
{
    if (getCIDR()<31)
            return (getBroadcast()-1);
    else
        return getBroadcast();
}

quint32 Subnet_v4::getFirstUsableIP()
{
    if (getCIDR()<31)
            return (getIP()+1);
    else
        return getIP();
}

quint32 Subnet_v4::getWildcard()
{
    return ~(*_netmask);
}

quint32 Subnet_v4::getBroadcast()
{
    return (getIP()|getWildcard());
}

quint32 Subnet_v4::getSize()
{
    quint32 nm = getNM();
    quint32 wc = (~nm)+1;

    return wc;
}

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

QString Subnet_v4::IP2String(quint32 &ip)
{
    QString outp;


    // I'm using pointer aritmethics to calculate the netmask octets...
    unsigned char oct[4];

    // select the single bytes through shifting and mask all irrelevant bits by ANDing 0xFF.
    oct[0] = ip & 0xFF;
    oct[1] = (ip >> 8) & 0xFF;
    oct[2] = (ip >> 16) & 0xFF;
    oct[3] = (ip >> 24) & 0xFF;

    // Now put it in the string which we will return afterwards.
    outp.sprintf("%u.%u.%u.%u",oct[3],oct[2],oct[1],oct[0]);

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

QString Subnet_v4::toString()
{
    QString outp;

    outp+=IP2String(getIP());
    outp+="/";

    QString cidr;
    cidr.setNum(getCIDR());
    outp+=cidr;

    return outp;
}

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

     qDebug("---DUMP START----------------------[IPv4]---");
     qDebug(" Object Address:         %p",this);
     qDebug("--------------------------------------------");
     qDebug(" Network:                %s",qPrintable(this->toString()));
     qDebug(" Netmask:                %s",qPrintable(IP2String(getNM())));
     qDebug(" Wildcard Bits:          %s",qPrintable(IP2String(wildcard)));
     qDebug(" Size:                   %u",getSize());
     qDebug(" First Usable IP:        %s",qPrintable(IP2String(first)));
     qDebug(" Last Usable IP:         %s",qPrintable(IP2String(last)));
     qDebug(" Broadcast Address:      %s",qPrintable(IP2String(broadcast)));
     qDebug("--------------------------------------------");
     qDebug(" Identifier:");
     qDebug("   %s",qPrintable(getIdentifier()));
     qDebug(" Description:");
     qDebug("   %s",qPrintable(getDescription()));
     qDebug("--------------------------------------------");
     qDebug(" Notes dump:");
     qDebug("   %s",qPrintable(getNotes()));
     qDebug("--------------------------------------------");
     qDebug(" Subnet color:           %s",qPrintable(_color->name()));
     qDebug("---------------------------------DUMP END---");
}

bool Subnet_v4::overlapsWith(Subnet_v4 &other_subnet)
{
    if (this==&other_subnet) return false;
    if (containsHost(other_subnet.getIP())) return true;
    if (other_subnet.containsHost(this->getIP())) return true;
    return false;
}

bool Subnet_v4::isLessThan(Subnet *subnet)
{
    // We consider IPv6 something less advanced than IPv4. ;)
    if (subnet->isV6()) return true;

    // I'm using pointer aritmethics to calculate the netmask octets...
    // get the ip into something we cannot destroy by accident. ;)
    unsigned long int ul_myip = getIP();
    unsigned long int ul_otherip = ((Subnet_v4*)subnet)->getIP();
    // Now we do the bad stuff. Get the ptr to the first byte of the ulong above.
    unsigned char* ptr1 = (unsigned char*)&ul_myip;
    unsigned char* ptr2 = (unsigned char*)&ul_otherip;
    // now use this address as an offset to select the subsequent bytes from the long.
    // Don't forget to use clean casting, or the compiler will get unhappy.
    unsigned char a1 = *(unsigned char*)(ptr1+3);
    unsigned char b1 = *(unsigned char*)(ptr1+2);
    unsigned char c1 = *(unsigned char*)(ptr1+1);
    unsigned char d1 = *(unsigned char*)(ptr1);
    unsigned char a2 = *(unsigned char*)(ptr2+3);
    unsigned char b2 = *(unsigned char*)(ptr2+2);
    unsigned char c2 = *(unsigned char*)(ptr2+1);
    unsigned char d2 = *(unsigned char*)(ptr2);


    // check bytewise if we are really less than this self designated superior subnet. We'll show that network-nazi what we are made of! ;)
    if (a1<a2) {
        // *gasp* e-p-i-c f-a-i-l.
        return true;
    } else if (a1==a2) {
        if (b1<b2) {
            // Ugh. Get offa me lawn. Me not loosing, not with alive me!
            return true;
        } else if (b1==b2) {
            if (c1<c2) {
                // Duh. Darn racists! Me not inferior!
                return true;
            } else if (c1==c2) {
                if (d1<d2) {
                    // Ok, its only a little bit superior. Nothing serious.
                    return true;
                }
            }
        }
    }

    // HAH! In your FACE! We are something *better* than him!!! ;)
    return false;
}

QString Subnet_v4::getStrWC()
{
    quint32 momWC = getWildcard();
    return IP2String(momWC);
}

QString Subnet_v4::getStrNM()
{
    quint32 momNM = getNM();
    return IP2String(momNM);
}
