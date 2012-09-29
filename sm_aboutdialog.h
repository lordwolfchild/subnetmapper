#ifndef SM_ABOUTDIALOG_H
#define SM_ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class SM_AboutDialog;
}

class SM_AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SM_AboutDialog(QWidget *parent = 0);
    ~SM_AboutDialog();
    
private:
    Ui::SM_AboutDialog *ui;
};

#endif // SM_ABOUTDIALOG_H
