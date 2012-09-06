#ifndef SUBNET_H
#define SUBNET_H

#include <QObject>
#include <QColor>

// Mainly taken from SubnetMapper Project that was developed under cocoa. May need some adjustments now.


// IPv4 Base subnet Class Definition
class Subnet : public QObject
{
    Q_OBJECT
public:
    explicit Subnet(QObject *parent = 0);
};

#endif // SUBNET_H
