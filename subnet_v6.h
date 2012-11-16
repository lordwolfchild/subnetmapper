#ifndef SUBNET_V6_H
#define SUBNET_V6_H

#include "subnet.h"
#include <QPair>
#include "subnet_v4.h"

/*

 TODO: Implement this. At the time, this class is just a copy of the IPv4 Subnet Class.
 I do not even think, that this is a very good idea to inherit Subnet like this. Maybe
 a restructurization would be in order? Maybe when issuing a model which uses polymorphic
 methods for storing and/or accessing its data...

 Technically the implementation of IPv6 calculations is very similar to IPv4. The
 arithmetics are the same, just the address space has quadrupled to a 128bit string.

 We do not have a datatype for that. As modern processors have a 64bit address space,
 there is simply no room for 128bit long numbers in the cpu registers. So we must emulate
 the correct behaviour. As we do not need features like bit carrying or stuff like that,
 and bitwise operations are atomic to each single bit we can just split the number into
 two 64bit quint64 variables and do the calculations twice for each part of the ip
 address string. This should work pretty well, reducing the big problem to writing
 parser routines for parsing a readable IPv6 address to our variables even in reduced
 form. Other variatons come in mind, such as a 16 unsigned chars in an array or a
 record of the same content, like Q_IPV6ADDR. But the method of the two quint64 vars
 should be quicker to implement, because we already have the methods with calculations
 for one quint32 in the IPv4 implementation. This method should be faster too, because
 the calculations are not atomic, but broader in nature - eight byte in one tick instead
 of one byte per tick - that is of course a line of thought without taking the compiler
 optimizations into consideration.

 I would say there should be three steps involved:

 1.) check validity of the address string (regexp)
 2.) undo the reducement to a normalized for of an IPv6 address.
 3.) parse this normalized form, which should be trivial.

 Divided. Time to conquer.

*/

class Subnet_v6 : public Subnet
{
    Q_OBJECT
public:
    explicit Subnet_v6(QObject *parent = 0);
    ~Subnet_v6();

    explicit Subnet_v6(QPair<quint64,quint64> ip, QPair<quint64,quint64> nm, QString id = "n/a", QString description = "n/a", QString notes = "n/a", QObject *parent = 0);
    explicit Subnet_v6(QString cidr = "::/128", QString id = "n/a", QString description = "n/a", QString notes = "n/a", QObject *parent = 0);

    void setIP(QPair<quint64,quint64> &ip);
    void setIP(QString &ip);
    void setNM(QPair<quint64,quint64> &nm);
    void setNM(QString &nm);
    void setNM(quint32 cidr);
    void setDescription(QString &description);
    void setIdentifier(QString &identifier);
    void setSelected(bool selected);
    void setNotes(QString &notes);

    IPversion getIPversion();
    QPair<quint64,quint64> getIP();
    QPair<quint64,quint64> getLastUsableIP();
    QPair<quint64,quint64> getFirstUsableIP();
    QPair<quint64,quint64> getWildcard();
    QPair<quint64,quint64> getBroadcast();
    quint64 getCIDR();
    quint64 getCIDR24Blocks();
    QPair<quint64,quint64> getNM();
    QString& getDescription();
    QString& getIdentifier();
    QString& getNotes();
    bool&    getSelected();

    QString getStrNM();
    QString getStrWC();

    static QString IP2String(QPair<quint64,quint64> &ip);
    static QPair<quint64,quint64> String2IP(QString &str_ip);
    static QString normalizeIP(QString &ip);
    static QString reduceIP(QString ip);

    QString toString();

    bool    containsHost(QPair<quint64,quint64> &host);

    void dumpAll();

    bool overlapsWith(Subnet_v6 &other_subnet);

    bool isLessThan(Subnet* subnet);

signals:

public slots:

protected:

    void normalize();

    // these two will be obsolete soon...
    quint32 *_ip_address;
    quint32 *_netmask;

    // instead those four will be relevant:
    // low and high part of ip address/netmask
    quint64 *_ip_address_lo;
    quint64 *_ip_address_hi;
    quint64 *_netmask_lo;
    quint64 *_netmask_hi;


    QString *_identifier;
    QString *_description;

    QString *_notes;

    bool    *_selected;


};

#endif // SUBNET_V6_H
