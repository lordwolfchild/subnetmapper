#ifndef SM_IPV6EDITDIALOG_H
#define SM_IPV6EDITDIALOG_H

#include <QDialog>
#include "subnet_v6.h"
#include "ipv6validator.h"

namespace Ui {
class SM_IPv6EditDialog;
}

class SM_IPv6EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SM_IPv6EditDialog(QWidget *parent = 0);
    ~SM_IPv6EditDialog();

    QString getIP();
    QString getNM();

    void setIP(QString ip);
    void setNM(QString nm);

    QString getDescription();
    QString getIdentifier();
    void setDescription(QString description);
    void setIdentifier(QString identifier);

    QColor getColor();
    void setColor(QColor &color);

protected slots:
    void updateFields();
    void updateCIDR();
    void colorClicked();

protected:
    Subnet_v6* helperSubnet;
    IPv6Validator validator;

private:
    Ui::SM_IPv6EditDialog *ui;
    QColor selectedColor;
};

#endif // SM_IPV6EDITDIALOG_H
