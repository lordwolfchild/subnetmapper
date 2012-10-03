#ifndef SM_IPV4EDITDIALOG_H
#define SM_IPV4EDITDIALOG_H

#include <QDialog>
#include "subnet_v4.h"

namespace Ui {
class sm_IPv4EditDialog;
}

class sm_IPv4EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sm_IPv4EditDialog(QWidget *parent = 0);
    ~sm_IPv4EditDialog();

    QString getIP();
    QString getNM();

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
    Subnet_v4 helperSubnet;

private:
    Ui::sm_IPv4EditDialog *ui;
    QColor selectedColor;
};

#endif // SM_IPV4EDITDIALOG_H
