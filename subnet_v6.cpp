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

    _ip_address_lo=new(quint64);
    _netmask_lo=new(quint64);
    _ip_address_hi=new(quint64);
    _netmask_hi=new(quint64);
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

    _ip_address_lo=new(quint64);
    _netmask_lo=new(quint64);
    _ip_address_hi=new(quint64);
    _netmask_hi=new(quint64);
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


// returns ::/128 if string is not parseable. Attention: returns ::/128, if IP reads ::/128 too! :)
// Throws away the CIDR Information.
QPair<quint64,quint64> Subnet_v6::String2IP(QString &str_ip)
{
    // QPair to store result in:
    QPair<quint64,quint64> resulting_ip;

    // normalize the IP and make sure it is valid and stuff. Will make sure that invalid input returns as ::/128.
    QString inp = normalizeIP(str_ip);

    // Because of normalization we do know inp a valid address. Now make sure there is a CIDR notation at the end. If so, split it off and
    // store it away for later reattachment.
    if (inp.contains(QRegExp("^([0-9a-f]{4,4}[:]){7,7}[0-9a-f]{4,4}(/[1-9][0-9]{0,2})$",Qt::CaseSensitive)))
    {
        QStringList cidrsplit = inp.split("/",QString::KeepEmptyParts,Qt::CaseSensitive);

        inp=cidrsplit.at(0);
    };

    QStringList syllableList = inp.split(':',QString::KeepEmptyParts,Qt::CaseSensitive);

    for (int i=0;i<8;i++) {

        //fetch syllable from List. so we iterate from first to last syllable through this loop.
        QString syllable = syllableList.at(i);

        bool conv_result;

        // we have to split it up to two variables. so a littel index magic is necessary.
        // furtheron, we use the conversion function from QString to base16-convert the
        // string to numerical values and cast them into our designated result bytes.
        if (i<4) *(((quint16*)&(resulting_ip.first))+i)=syllable.toUInt(&conv_result,16);
        else *(((quint16*)&(resulting_ip.second))+(i-4))=syllable.toUInt(&conv_result,16);

    }

    return resulting_ip;
}

QString Subnet_v6::normalizeIP(QString &ip)
{
    // takes the input ip, slashes whitespaces of the end and start, expands reduced
    // parts of the ip and makes sure everything is in order with the ip. returns a
    // normalized zeroed ip in string representation if anything goes wrong.


    // take the input string, cut off the trailing/prepending whitespaces and convert to lowercase.
    QString preinp=ip.trimmed().toLower();

    // stores the string that is going to be normalized after CIDR processing
    QString inp;

    // stores the normalized version of the string for output.
    QString outp="";

    // stores the info if CIDR notation is used and of course the CIDR itself
    bool isCIDR=false;
    uint CIDR=0;

    // check for validity of the input. we are sort of nice to the user, so we accept
    // multiple input formats. We take whitespace polluted strings in normal, reduced and CIDR notations.
    // This regexpr checks, if the already cleaned up, lowercased string contains a valid IPv6 address.
    // If not, just return a zeroed address (no address, ::/128, http://de.wikipedia.org/wiki/IPv6#Besondere_Adressen)
    if (!preinp.contains(QRegExp("^([0-9a-f]{0,4}[:]){2,7}[0-9a-f]{0,4}(/[1-9][0-9]{0,2}){0,1}$",Qt::CaseSensitive)))
    {
        outp="0000:0000:0000:0000:0000:0000:0000:0000";
        return outp;
    }

    // Ok, now we know its a valid address. Now make sure there is a CIDR notation at the end. If so, split it off and
    // store it away for later reattachment.
    if (preinp.contains(QRegExp("^([0-9a-f]{0,4}[:]){2,7}[0-9a-f]{0,4}(/[1-9][0-9]{0,2})$",Qt::CaseSensitive)))
    {
        QStringList cidrsplit = preinp.split("/",QString::KeepEmptyParts,Qt::CaseSensitive);

        inp=cidrsplit.at(0);
        CIDR=cidrsplit.at(1).toUInt();
        isCIDR=true;

    } else inp=preinp;

    // count the number of colons in our address. This will be important for expansion of reduced parts of the IP.
    int colon_count=inp.count(":");

    // now split the IP into parts that are parted by colons.
    QStringList tokens = inp.split(":",QString::KeepEmptyParts,Qt::CaseSensitive);

    // a little helper, stores the active 'syllable' of the IP that we will construct.
    int counter=0;

    // as long as there is a remaining part of the input ip...
    while (!tokens.isEmpty())  {

        // ...get the next syllable...
        QString token = tokens.first();
        tokens.removeFirst();

        // ...and if it is empty, expand the reduced part of the IP.
        if (token.isEmpty()&(counter>0)&!tokens.empty())
        {
            int reducement_cnt=8-colon_count;
            // for each skipped syllable we have to append a zeroed one and increment the syllable counter.
            for (int i=0;i<reducement_cnt;i++)
            {
                token.append("0000:");
                counter++;
            };

        // if there is something in this syllable, fill it up to four octets.
        } else {
            while (token.length()<4) token.prepend('0');
            if (counter<7) token.append(':');
        }

        // append it to the output and increment the syllable counter.
        counter++;
        outp+=token;
    }

    // now append the cidr part if there is one.
    if (isCIDR) outp+=(QString("/")+QString::number(CIDR));

    // Check for consitency a last time
    if (!(outp.count(':')==7)) outp="0000:0000:0000:0000:0000:0000:0000:0000";

    // ...finished!
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

