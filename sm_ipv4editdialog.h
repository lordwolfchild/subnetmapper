#ifndef SM_IPV4EDITDIALOG_H
#define SM_IPV4EDITDIALOG_H

#include <QDialog>

namespace Ui {
class sm_IPv4EditDialog;
}

class sm_IPv4EditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit sm_IPv4EditDialog(QWidget *parent = 0);
    ~sm_IPv4EditDialog();
    
private:
    Ui::sm_IPv4EditDialog *ui;
};

#endif // SM_IPV4EDITDIALOG_H
