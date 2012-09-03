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
    
    void setIP(quint32 &ip);
    void setNM(quint32 &nm);
    void setDescription(QString &description);
    void setIdentifier(QString &identifier);
    void setSelected(bool &selected);
    void setColor(QColor &color);
    void setNotes(QString &notes);

    quint32 &getIP();
    quint32 &getNM();
    QString &getDescription();
    QString &getIdentifier();
    QString &getNotes();
    bool    &getSelected();
    QColor  &getColor();


signals:
    
public slots:

protected:

    quint32 _ip_address_int;
    quint32 _netmask_int;

    QString _identifier;
    QString _description;

    QColor  _color;
    QString _notes;

    bool    _selected;
};

#endif // SUBNET_H
