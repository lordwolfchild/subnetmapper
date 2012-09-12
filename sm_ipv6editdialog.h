#ifndef SM_IPV6EDITDIALOG_H
#define SM_IPV6EDITDIALOG_H

#include <QDialog>

namespace Ui {
class SM_IPv6EditDialog;
}

class SM_IPv6EditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SM_IPv6EditDialog(QWidget *parent = 0);
    ~SM_IPv6EditDialog();
    
private:
    Ui::SM_IPv6EditDialog *ui;
};

#endif // SM_IPV6EDITDIALOG_H
