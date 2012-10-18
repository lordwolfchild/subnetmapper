#include "subnet_v6.h"
#include <QRegExp>

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
    _color=new QColor("lightblue");

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
    _color=new QColor("lightblue");

    *_ip_address_hi=ip.first;
    *_ip_address_lo=ip.second;
    *_netmask_hi=nm.first;
    *_netmask_lo=nm.second;
    setDescription(description);
    setIdentifier(id);
    setNotes(notes);

    normalize();

}

Subnet_v6::Subnet_v6(QString cidr, QString id, QString description, QString notes, QObject *parent) :
    Subnet(parent)
{

    // normalize and clean up input
    QString inp=normalizeIP(cidr);

    // stores the info if CIDR notation is used and of course the CIDR itself. Setting it to 128 guarantees a valid object even if cidr notation is ommited.
    quint32 CIDR=128;

    // Ok, we know its a valid address (through normalizeIP()). Now make sure there is a CIDR notation at the end. Split it off and
    // store it away for later reattachment.
    if (inp.contains(QRegExp("^([0-9a-f]{0,4}[:]){2,7}[0-9a-f]{0,4}(/[1-9][0-9]{0,2})$",Qt::CaseSensitive)))
    {
        QStringList cidrsplit = inp.split("/",QString::KeepEmptyParts,Qt::CaseSensitive);

        inp=cidrsplit.at(0);
        CIDR=cidrsplit.at(1).toUInt();
    }

    _ip_address_lo=new(quint64);
    _netmask_lo=new(quint64);
    _ip_address_hi=new(quint64);
    _netmask_hi=new(quint64);
    _description=new(QString);
    _notes=new(QString);
    _identifier=new(QString);
    _selected=new(bool);
    _color=new QColor("lightblue");

    QPair<quint64,quint64> momip = String2IP(inp);

    *_ip_address_hi=momip.first;
    *_ip_address_lo=momip.second;
    setDescription(description);
    setIdentifier(id);
    setNotes(notes);

    setNM(CIDR);


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

void Subnet_v6::setNM(quint32 cidr)
{
    // Ok, the splitting in two 64 bit vars makes this special.

    // if nm is higher than 64, set all higher bits 1, and shift your way through the lower bits...
    if (cidr>64)
    {
        *_netmask_hi=~((quint64)0);
        *_netmask_lo=~((quint64)0);
        *_netmask_lo=*_netmask_lo<<(64-(cidr-64));

    // if not, just zero our the lower bits and shift the higher bits.
    } else  {
        *_netmask_hi=~((quint64)0);
        *_netmask_hi=*_netmask_hi<<(64-(cidr));
        *_netmask_lo=0;
    }

    // now we can normalize the network address.
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

void Subnet_v6::setNotes(QString &notes)
{
        *_notes=notes;
}

QPair<quint64, quint64> Subnet_v6::getIP()
{
    return (QPair<quint64,quint64>)qMakePair(*_ip_address_hi,*_ip_address_lo);
}


QPair<quint64,quint64> Subnet_v6::getLastUsableIP()
{
    // If Network is big enough to hold at least two regular IPs, return last one by subtracting one from the broadcast IP.
    if (getCIDR()<127)
    {
        QPair<quint64,quint64> mombc = getBroadcast();
        mombc.second-=1;
        return mombc;
    }
    else
        return getIP();
}


QPair<quint64,quint64> Subnet_v6::getFirstUsableIP()
{

    //  If Network is big enough to hold at least two regular IPs, return first one by adding one to the network address.
    if (getCIDR()<127)
    {
        QPair<quint64,quint64> momfirst = getIP();
        momfirst.second+=1;
        return momfirst;
    }
    else
        return getIP();
 }


QPair<quint64,quint64> Subnet_v6::getWildcard()
{

    // Wildcard is NOT Netmask.
    QPair<quint64,quint64> momnm = getNM();
    momnm.first=(~momnm.first);
    momnm.second=(~momnm.second);

    return momnm;
}


QPair<quint64,quint64> Subnet_v6::getBroadcast()
{
    QPair<quint64,quint64> momip = getIP();
    QPair<quint64,quint64> momwc = getWildcard();
    QPair<quint64,quint64> mombc;

    // broadcast is ip bitwise-OR wildcard. Return.
    mombc.first=momip.first|momwc.first;
    mombc.second=momip.second|momwc.second;

    return mombc;
}

quint64 Subnet_v6::getCIDR()
{
    // stores the return value.
    unsigned char cidr=0;

    QPair<quint64,quint64> momNM = getNM();

    // Principle: Store a temp copy of netmask.
    // now left shift both parts of the netmask
    // until both are zero. count the steps.
    // After all bits have been reset, the count of steps
    // equals the CIDR.
    while (momNM.first>0) {
        cidr++;
        momNM.first=momNM.first<<1;
    }
    while (momNM.second>0) {
        cidr++;
        momNM.second=momNM.second<<1;
    }

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

QString Subnet_v6::IP2String(QPair<quint64,quint64> &ip)
{
    // stores output String
    QString outp;

    // a byte array for conversion
    unsigned char mombytes[16] = {0,0,0,0,
                                  0,0,0,0,
                                  0,0,0,0,
                                  0,0,0,0};
    // temp ip storage for conversion convenience
    quint64 hi = ip.first;
    quint64 lo = ip.second;

// Lets tell those guys out there that we did unholy stuff with pointers which garbles their IPv6 address display

#ifndef __ORDER_LITTLE_ENDIAN__
#warning "SubnetMapper only works well on litte endian Systems. Reimplement conversion functions of Subnet_v6 for big endian systems. Display of IPv6 Addresses will be false."
#endif

    // some casting magic for bytewise conversion. I know this could be done with more style, but we leave optimization stuff to the compiler for now.
    mombytes[0]=*(((unsigned char*)&hi)+7);
    mombytes[1]=*(((unsigned char*)&hi)+6);
    mombytes[2]=*(((unsigned char*)&hi)+5);
    mombytes[3]=*(((unsigned char*)&hi)+4);
    mombytes[4]=*(((unsigned char*)&hi)+3);
    mombytes[5]=*(((unsigned char*)&hi)+2);
    mombytes[6]=*(((unsigned char*)&hi)+1);
    mombytes[7]=*(((unsigned char*)&hi));
    mombytes[8]=*(((unsigned char*)&lo)+7);
    mombytes[9]=*(((unsigned char*)&lo)+6);
    mombytes[10]=*(((unsigned char*)&lo)+5);
    mombytes[11]=*(((unsigned char*)&lo)+4);
    mombytes[12]=*(((unsigned char*)&lo)+3);
    mombytes[13]=*(((unsigned char*)&lo)+2);
    mombytes[14]=*(((unsigned char*)&lo)+1);
    mombytes[15]=*(((unsigned char*)&lo));

    // now use this stuff to output the string representation.
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
    quint64 numberOfCIDRBlocks=(((~momnm.second)>>8)+1);

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

        // we have to split it up to two variables. so a little index magic is necessary.
        // furtheron, we use the conversion function from QString to base16-convert the
        // string to numerical values and cast them into our designated result bytes.
        // We have to care about endianess here! So this works only on little endian CPUs.
        if (i<4) *(((quint16*)&(resulting_ip.first))+(3-i))=syllable.toUInt(&conv_result,16);
        else *(((quint16*)&(resulting_ip.second))+(3-(i-4)))=syllable.toUInt(&conv_result,16);

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

    // fetch the number of valid syllables, we will overwrite this later with a mor ecomplete set of syllables.
    QStringList tokens = inp.split(":",QString::SkipEmptyParts,Qt::CaseSensitive);

    QString expansionHelper=":";
    for (int i=0;i<8-tokens.count();i++) expansionHelper+="0:";

    inp.replace("::",expansionHelper);
    if (inp.at(0)==':') inp.remove(0,1);
    if (inp.at(inp.length()-1)==':') inp.remove(inp.length()-1,1);

    // split it up again so we can expand the single syllables.
    tokens = inp.split(":",QString::SkipEmptyParts,Qt::CaseSensitive);

    // as long as there is a remaining part of the input ip...
    for (int i=0;i<tokens.count();i++) {

        // ...get the next syllable...
        QString token = tokens.at(i);

        // if there is something in this syllable, fill it up to four octets.
        while (token.length()<4) token.prepend('0');
        if (i<tokens.count()-1) token.append(':');

        // append it to the output and increment the syllable counter.
        outp+=token;
    }

    // now append the cidr part if there is one.
    if (isCIDR) outp+=(QString("/")+QString::number(CIDR));

    // Check for consistency a last time
    if (!(outp.count(':')==7)) outp="0000:0000:0000:0000:0000:0000:0000:0000";

    // ...finished!
    return outp;
}

// reduces zero repetitions according to standard. At least tries to be efficient in doing so.
QString Subnet_v6::reduceIP(QString ip)
{

    // take the input string, cut off the trailing/prepending whitespaces and convert to lowercase.
    QString inp=ip.trimmed().toLower();

    // stores the normalized version of the string for output.
    QString outp="";

    // stores the info if CIDR notation is used and of course the CIDR itself
    bool isCIDR=false;
    uint CIDR=0;

    // check for validity of the input. we are sort of nice to the user, so we accept
    // multiple input formats. We take whitespace polluted strings in normal, reduced and CIDR notations.
    // This regexpr checks, if the already cleaned up, lowercased string contains a valid IPv6 address.
    // If not, just return a zeroed address (no address, ::/128, http://de.wikipedia.org/wiki/IPv6#Besondere_Adressen)
    if (!inp.contains(QRegExp("^([0-9a-f]{4,4}[:]){7,7}[0-9a-f]{4,4}(/[1-9][0-9]{0,2}){0,1}$",Qt::CaseSensitive)))
    {
        outp="0000:0000:0000:0000:0000:0000:0000:0000";
        return outp;
    }

    // Ok, now we know its a valid address. Now make sure there is a CIDR notation at the end. If so, split it off and
    // store it away for later reattachment.
    if (inp.contains(QRegExp("^([0-9a-f]{4,4}[:]){7,7}[0-9a-f]{4,4}(/[1-9][0-9]{0,2})$",Qt::CaseSensitive)))
    {
        QStringList cidrsplit = inp.split("/",QString::KeepEmptyParts,Qt::CaseSensitive);

        CIDR=cidrsplit.at(1).toUInt();
        isCIDR=true;
        inp=cidrsplit.at(0);

    };

    // Ok, we have a valid address without CIDR now. time to reduce it. first we have to split it up.
    QStringList syllables = inp.split(":",QString::KeepEmptyParts,Qt::CaseSensitive);

    for (int i=0;i<syllables.count();i++) {
        QString syllable=syllables.at(i);

        while ((syllable.at(0)=='0')&(syllable.length()>1)) syllable.remove(0,1);

        outp+=syllable;
        if (i<7) outp+=":";

    }

    QRegExp re = QRegExp("(0[:]){1,7}[0]{0,1}");
    bool foundSome = outp.contains(re);


    uint indexOfLongestMatch=0;
    uint lengthOfLongestMatch=0;
    bool foundReducableString=false;

    QStringList matches = re.capturedTexts();
    if (foundSome) {

        foundReducableString=true;

        for (int i=0;i<matches.count();i++) {
            if ((matches.at(i).length())>lengthOfLongestMatch) {
                lengthOfLongestMatch=matches.at(i).length();
                indexOfLongestMatch=i;
            }
        }
    }

    if (foundReducableString) {
        outp.replace(matches.at(indexOfLongestMatch),"::");
    }

    if (outp==":") outp="::";

    // now append the cidr part if there is one.
    if (isCIDR) outp+=(QString("/")+QString::number(CIDR));

    return outp;

}

QString Subnet_v6::toString()
{
    QString outp;
    QPair<quint64,quint64> momip = getIP();
    outp+=reduceIP(IP2String(momip));
    outp+="/";

    QString cidr;
    cidr.setNum(getCIDR());
    outp+=cidr;

    return outp;
}


bool   Subnet_v6::containsHost(QPair<quint64,quint64> &host)
{
    // make sure the network is noralized, otherwise this function will not give correct answers.
    normalize();

    QPair<quint64,quint64> momip = getIP();
    QPair<quint64,quint64> momnm = getNM();

    // a host ip ANDed with the netmask of its network equals its network address. If so, return true, else false.
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

    // Just receive object data and output for debugging purposes.
     QPair<quint64,quint64>  wildcard=getWildcard();
     QPair<quint64,quint64>  netmask=getNM();
     QPair<quint64,quint64>  first = getFirstUsableIP();
     QPair<quint64,quint64>  last = getLastUsableIP();
     QPair<quint64,quint64>  broadcast = getBroadcast();


     qDebug("---DUMP START----------------------------------------------[IPv6]---");
     qDebug(" Object Address:         %p",this);
     qDebug("--------------------------------------------------------------------");
     qDebug(" Network:                %s",qPrintable(this->toString()));
     qDebug(" Netmask:                %s",qPrintable(reduceIP(IP2String(netmask))));
     qDebug(" Wildcard Bits:          %s",qPrintable(reduceIP(IP2String(wildcard))));
     qDebug(" Size: (only up to /64!) %llu",wildcard.second);
     qDebug(" First Usable IP:        %s",qPrintable(reduceIP(IP2String(first))));
     qDebug(" Last Usable IP:         %s",qPrintable(reduceIP(IP2String(last))));
     qDebug(" Broadcast Address:      %s",qPrintable(reduceIP(IP2String(broadcast))));
     qDebug("--------------------------------------------------------------------");
     qDebug(" Identifier:");
     qDebug("   %s",qPrintable(getIdentifier()));
     qDebug(" Description:");
     qDebug("   %s",qPrintable(getDescription()));
     qDebug("--------------------------------------------------------------------");
     qDebug(" Notes dump:");
     qDebug("   %s",qPrintable(getNotes()));
     qDebug("--------------------------------------------------------------------");
     qDebug(" Subnet color:           %s",qPrintable(_color->name()));
     qDebug("---------------------------------------------------------DUMP END---");
}

bool Subnet_v6::overlapsWith(Subnet_v6 &other_subnet)
{
    QPair<quint64,quint64> myIP  = this->getIP();
    QPair<quint64,quint64> hisIP = other_subnet.getIP();


    if (containsHost(hisIP)) return true;
    if (other_subnet.containsHost(myIP)) return true;
    return false;
}

bool Subnet_v6::isLessThan(Subnet *subnet)
{

    if (subnet->isV4()) return false;

    return (toString()<(subnet->toString()));
}

QString Subnet_v6::getStrWC()
{
    QPair<quint64,quint64> momWC = getWildcard();
    return IP2String(momWC);
}

QString Subnet_v6::getStrNM()
{
    QPair<quint64,quint64> momNM = getNM();
    return IP2String(momNM);
}


