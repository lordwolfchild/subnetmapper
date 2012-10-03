#ifndef SUBNET_V4_H
#define SUBNET_V4_H

#include <QObject>
#include <QColor>
#include "subnet.h"

// Mainly taken from SubnetMapper Project that was developed under cocoa. May need some adjustments now.


// IPv4 Base subnet Class Definition
class Subnet_v4 : public Subnet
{
    Q_OBJECT
public:
    explicit Subnet_v4(QObject *parent = 0);
    ~Subnet_v4();

    explicit Subnet_v4(quint32 ip, quint32 nm = ~0, QString id = "n/a", QString description = "n/a", QString notes = "n/a", QObject *parent = 0);
    explicit Subnet_v4(QString ip, QString nm = "255.255.255.255", QString id = "n/a", QString description = "n/a", QString notes = "n/a", QObject *parent = 0);

    void setIP(quint32 &ip);
    void setIP(QString &ip);
    void setNM(quint32 &nm);
    void setNM(QString &nm);
    void setDescription(QString &description);
    void setIdentifier(QString &identifier);
    void setSelected(bool &selected);
    void setNotes(QString &notes);

    IPversion getIPversion();
    quint32& getIP();
    quint32 getLastUsableIP();
    quint32 getFirstUsableIP();
    quint32 getWildcard();
    quint32 getBroadcast();
    quint32 getSize();
    quint32 getCIDR();
    quint32 getCIDR24Blocks();
    quint32& getNM();
    QString& getDescription();
    QString& getIdentifier();
    QString& getNotes();
    bool&    getSelected();

    QString getStrNM();
    QString getStrWC();

    static QString IP2String(quint32 &ip);
    static quint32 String2IP(QString &str_ip);

    QString toString();

    bool    containsHost(quint32 &host);

    void dumpAll();

    bool overlapsWith(Subnet_v4 &other_subnet);

signals:

public slots:

protected:

    void normalize();

    quint32 *_ip_address;
    quint32 *_netmask;

    QString *_identifier;
    QString *_description;

    QString *_notes;

    bool    *_selected;
};

#endif // SUBNET_V4_H
