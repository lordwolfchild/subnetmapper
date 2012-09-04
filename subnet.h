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
    ~Subnet();
    
    void setIP(quint32 &ip);
    void setIP(QString &ip);
    void setNM(quint32 &nm);
    void setNM(QString &nm);
    void setDescription(QString &description);
    void setIdentifier(QString &identifier);
    void setSelected(bool &selected);
    void setColor(QColor &color);
    void setNotes(QString &notes);

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
    QColor&  getColor();

    static QString IP2String(quint32 &ip);
    static quint32 String2IP(QString &str_ip);

    QString toString();

    bool    containsHost(quint32 &host);

    void dumpAll();

signals:
    
public slots:

protected:

    void normalize();

    quint32 *_ip_address;
    quint32 *_netmask;

    QString *_identifier;
    QString *_description;

    QColor  *_color;
    QString *_notes;

    bool    *_selected;
};

#endif // SUBNET_H
