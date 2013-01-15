#ifndef SM_CONFIGDIALOG_H
#define SM_CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class SM_ConfigDialog;
}

class SM_ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SM_ConfigDialog(QWidget *parent = 0);
    ~SM_ConfigDialog();

public slots:

    void accept();

private slots:
    void on_sn_autoloadBrowse_clicked();

    void on_sn_clearRecent_clicked();

private:
    Ui::SM_ConfigDialog *ui;
};

#endif // SM_CONFIGDIALOG_H
