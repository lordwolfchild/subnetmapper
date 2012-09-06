#include "subnet_v6.h"

Subnet_v6::Subnet_v6(QObject *parent) :
    Subnet(parent)
{
    _ip_address_lo=new(quint64);
    _netmask_lo=new(quint64);
    _ip_address_hi=new(quint64);
    _netmask_hi=new(quint64);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    quint64 momIp_lo = 1;
    quint64 momNm_lo = ~0;
    quint64 momIp_hi = 0;
    quint64 momNm_hi = ~0;

    QString mom = "n/a";

    *_ip_address_lo=momIp_lo;
    *_netmask_lo=momNm_lo;
    *_ip_address_hi=momIp_hi;
    *_netmask_hi=momNm_hi;
    setDescription(mom);
    setIdentifier(mom);
    setNotes(mom);

    normalize();
}

Subnet_v6::Subnet_v6(QPair<quint64,quint64> ip, QPair<quint64,quint64> nm, QString id, QString description, QString notes, QObject *parent) :
    Subnet(parent)
{

    _ip_address=new(quint32);
    _netmask=new(quint32);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    *_ip_address_hi=ip.first;
    *_ip_address_lo=ip.second;
    *_netmask_hi=nm.first;
    *_netmask_lo=nm.second;
    setDescription(description);
    setIdentifier(id);
    setNotes(notes);

    normalize();

}

Subnet_v6::Subnet_v6(QString ip, QString nm, QString id, QString description, QString notes, QObject *parent) :
    Subnet(parent)
{

    _ip_address=new(quint32);
    _netmask=new(quint32);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new(QColor);

    QPair<quint64,quint64> momip = String2IP(ip);
    QPair<quint64,quint64> momnm = String2IP(nm);
    *_ip_address_hi=momip.first;
    *_ip_address_lo=momip.second;
    *_netmask_hi=momnm.first;
    *_netmask_lo=momnm.second;
    setDescription(description);
    setIdentifier(id);
    setNotes(notes);

    normalize();

}

Subnet_v6::~Subnet_v6()
{
    delete(_ip_address_lo);
    delete(_netmask_lo);
    delete(_ip_address_hi);
    delete(_netmask_hi);
    delete(_description);
    delete(_notes);
    delete(_identifier);
    delete(_selected);
    delete(_color);
}

void Subnet_v6::setIP(QPair<quint64,quint64> &ip)
{
    *_ip_address_hi=ip.first;
    *_ip_address_lo=ip.second;
    normalize();
}

void Subnet_v6::setIP(QString &ip)
{
    QPair<quint64,quint64> momip  = String2IP(ip);
    *_ip_address_hi=momip.first;
    *_ip_address_lo=momip.second;
    normalize();
}

void Subnet_v6::setNM(QPair<quint64,quint64> &nm)
{
    *_netmask_hi=nm.first;
    *_netmask_lo=nm.second;
    normalize();
}

void Subnet_v6::setNM(QString &nm)
{
    QPair<quint64,quint64> momnm  = String2IP(nm);
    *_netmask_hi=momnm.first;
    *_netmask_lo=momnm.second;
    normalize();
}

void Subnet_v6::setDescription(QString &description)
{
    *_description=description;
}

void Subnet_v6::setIdentifier(QString &identifier)
{
    *_identifier=identifier;
}

void Subnet_v6::setSelected(bool &selected)
{
        *_selected=selected;
}

void Subnet_v6::setColor(QColor &color)
{
        *_color=color;
}

void Subnet_v6::setNotes(QString &notes)
{
        *_notes=notes;
}

QPair<quint64, quint64> Subnet_v6::getIP()
{
    return (QPair<quint64,quint64>)qMakePair(*_ip_address_hi,*_ip_address_lo);
}


// TODO
QPair<quint64,quint64> Subnet_v6::getLastUsableIP()
{
    if (getCIDR()<31)
    {
        QPair<quint64,quint64> mombc = getBroadcast();
        mombc.second-=1;
        return mombc;
    }
    else
        return qMakePair((quint64)0,(quint64)0);
}

// TODO
QPair<quint64,quint64> Subnet_v6::getFirstUsableIP()
{
    if (getCIDR()<31)
    {
        QPair<quint64,quint64> momfirst = getIP();
        momfirst.second+=1;
        return momfirst;
    }
    else
            return qMakePair((quint64)0,(quint64)0);
 }

// TODO
QPair<quint64,quint64> Subnet_v6::getWildcard()
{
    QPair<quint64,quint64> momnm = getNM();
    momnm.first=(~momnm.first);
    momnm.second=(~momnm.second);

    return momnm;
}

// TODO
QPair<quint64,quint64> Subnet_v6::getBroadcast()
{
    QPair<quint64,quint64> momip = getIP();
    QPair<quint64,quint64> momwc = getWildcard();
    QPair<quint64,quint64> mombc;

    mombc.first=momip.first|momwc.first;
    mombc.second=momip.second|momwc.second;

    return mombc;
}

// TODO
quint64 Subnet_v6::getSize()
{
    // Ok, everybody who knows what they are doing is aware of the dragon here.
    // for the rest: this function is only valid for 64bit return values, which
    // is not nearly enough to display the biggest possible number of IP addresses
    // in a v6 block. But this would not be pratcical anyway in this program,
    // so we live with it.

    QPair<quint64,quint64> momnm= getNM();
    return (~momnm.second)+1;

}

// TODO
quint64 Subnet_v6::getCIDR()
{
    // same thing as above, not perfect for all possible nets of IPv6.
    // but we live with it.

    unsigned char cidr=64;
    quint64 wildcard= (getNM()).second;

    while (wildcard>0) {
            cidr--;
            wildcard>>=1;
    };

    return cidr;

}

Subnet_v6::IPversion Subnet_v6::getIPversion()
{
    return Subnet_v6::IPv6;
}

QPair<quint64,quint64> Subnet_v6::getNM()
{
    QPair<quint64,quint64> momnm = qMakePair(*_netmask_hi,*_netmask_lo);
    return momnm;
}

QString& Subnet_v6::getDescription()
{
    return *_description;
}

QString& Subnet_v6::getIdentifier()
{
    return *_identifier;
}

QString& Subnet_v6::getNotes()
{
    return *_notes;
}

bool&    Subnet_v6::getSelected()
{
    return *_selected;
}

QColor&  Subnet_v6::getColor()
{
    return *_color;
}

QString Subnet_v6::IP2String(QPair<quint64,quint64> &ip)
{
    QString outp;

    unsigned char mombytes[16] = {0,0,0,0,
                                  0,0,0,0,
                                  0,0,0,0,
                                  0,0,0,0};
    quint64 hi = ip.first;
    quint64 lo = ip.second;

    mombytes[0]=*(((unsigned char*)&hi)+7);
    mombytes[1]=*(((unsigned char*)&hi)+6);
    mombytes[2]=*(((unsigned char*)&hi)+5);
    mombytes[3]=*(((unsigned char*)&hi)+4);
    mombytes[4]=*(((unsigned char*)&hi)+3);
    mombytes[5]=*(((unsigned char*)&hi)+2);
    mombytes[6]=*(((unsigned char*)&hi)+1);
    mombytes[7]=*((unsigned char*)&hi);
    mombytes[8]=*(((unsigned char*)&lo)+7);
    mombytes[9]=*(((unsigned char*)&lo)+6);
    mombytes[10]=*(((unsigned char*)&lo)+5);
    mombytes[11]=*(((unsigned char*)&lo)+4);
    mombytes[12]=*(((unsigned char*)&lo)+3);
    mombytes[13]=*(((unsigned char*)&lo)+2);
    mombytes[14]=*(((unsigned char*)&lo)+1);
    mombytes[15]=*((unsigned char*)&lo);

    outp.sprintf("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 mombytes[0],
                 mombytes[1],
                 mombytes[2],
                 mombytes[3],
                 mombytes[4],
                 mombytes[5],
                 mombytes[6],
                 mombytes[7],
                 mombytes[8],
                 mombytes[9],
                 mombytes[10],
                 mombytes[11],
                 mombytes[12],
                 mombytes[13],
                 mombytes[14],
                 mombytes[15]);

    return outp;
}

quint64 Subnet_v6::getCIDR24Blocks()
{
    QPair<quint64,quint64> momnm = getNM();

    // complement the netmask and shift it 8 bits to the right and you get the number of /24
    // nets in the Subnet_v6 (-1, because the zero counts a a net again).
    quint32 numberOfCIDRBlocks=(((~momnm.first)>>8)+1);

    return numberOfCIDRBlocks;
}


// returns zero if string is not parseable. Attention: returns zero, if IP reads 0.0.0.0, too! :)
QPair<quint64,quint64> Subnet_v6::String2IP(QString &str_ip)
{
 /*   // clean up the input
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
    } else return 0;*/
}

QString Subnet_v6::normalizeIP(QString &ip)
{
    // takes the input ip, slashes whitespaces of the end and start, expands reduced
    // parts of the ip and makes sure everything is in order with the ip. returns a
    // normalized zeroed ip if anything goes wrong.

    unsigned char mombytes[16] = {0,1,2,3,4,
                                  5,112,7,8,
                                  9,10,11,233,
                                  131,14,15};

    /*qDebug("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
           mombytes[0],
           mombytes[1],
           mombytes[2],
           mombytes[3],
           mombytes[4],
           mombytes[5],
           mombytes[6],
           mombytes[7],
           mombytes[8],
           mombytes[9],
           mombytes[10],
           mombytes[11],
           mombytes[12],
           mombytes[13],
           mombytes[14],
           mombytes[15]);*/

    QString mom=ip.trimmed();

    int colon_count=mom.count(":");

    QStringList tokens = mom.split(":",QString::KeepEmptyParts,Qt::CaseSensitive);

    QString outp="";

    while (!tokens.isEmpty())  {
        QString token = tokens.first();
        tokens.removeFirst();

        if (token.isEmpty()) {
            int i=7-colon_count;
            while (i>=0) { outp+="0000:"; i--; qDebug("0000: (formerly reduced)"); };
        } else {
            while (token.length()<4) token.prepend('0');
            if (!tokens.isEmpty()) token.append(':');
            outp+=token;
        };

        qDebug("%s",qPrintable(token));
    }

    return outp;
}

QString Subnet_v6::toStr()
{
    QString outp;
    QPair<quint64,quint64> momip = getIP();
    outp+=IP2String(momip);
    outp+="/";

    QString cidr;
    cidr.setNum(getCIDR());
    outp+=cidr;

    return outp;
}

// TODO
bool   Subnet_v6::containsHost(QPair<quint64,quint64> &host)
{
    normalize();

    QPair<quint64,quint64> momip = getIP();
    QPair<quint64,quint64> momnm = getNM();

    if (((host.first&momnm.first)==momip.first)&((host.second&momnm.second)==momip.second))
         return true;
    else
         return false;
}

void Subnet_v6::normalize()
{
    // as we want to specify a Subnet here, we actually have to make sure, that the ip entered is the network address,
    // not one of the network's host addresses. Just AND IP and NM like we learned in ancient times at school.
    *_ip_address_lo&=*_netmask_lo;
    *_ip_address_hi&=*_netmask_hi;

}

void Subnet_v6::dumpAll()
{
/*     quint64 wildcard=getWildcard();
     quint64 first = getFirstUsableIP();
     quint64 last = getLastUsableIP();
     quint64 broadcast = getBroadcast();*/
     quint64 cidr24 = getCIDR24Blocks();

     qDebug("---DUMP START----------------------[IPv4]---");
     qDebug(" Object Address:         %p",this);
     qDebug("--------------------------------------------");
     qDebug(" Network:                %s",qPrintable(this->toStr()));
//     qDebug(" Netmask:                %s",qPrintable(IP2String(getNM())));
//     qDebug(" Wildcard Bits:          %s",qPrintable(IP2String(wildcard)));
     qDebug(" Size:                   %llu",getSize());
//     qDebug(" First Usable IP:        %s",qPrintable(IP2String(first)));
//     qDebug(" Last Usable IP:         %s",qPrintable(IP2String(last)));
//     qDebug(" Broadcast Address:      %s",qPrintable(IP2String(broadcast)));
     qDebug(" CIDR24 Blocks:          %llu",cidr24);
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

