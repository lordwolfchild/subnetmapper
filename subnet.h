#ifndef SUBNET_H
#define SUBNET_H

#include <QObject>
#include <QColor>
#include <QString>

// Mainly taken from SubnetMapper Project that was developed under cocoa. May need some adjustments now.


// IPv4 Base subnet Class Definition
class Subnet : public QObject
{
    Q_OBJECT
public:
    explicit Subnet(QObject *parent = 0);

    virtual void dumpAll() = 0;
    virtual QString toString() = 0;

    enum IPversion { IPv4, IPv6 };

    virtual IPversion getIPversion() = 0;
};

#endif // SUBNET_H
