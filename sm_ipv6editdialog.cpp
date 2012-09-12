#include "sm_ipv6editdialog.h"
#include "ui_sm_ipv6editdialog.h"

SM_IPv6EditDialog::SM_IPv6EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SM_IPv6EditDialog)
{
    ui->setupUi(this);
}

SM_IPv6EditDialog::~SM_IPv6EditDialog()
{
    delete ui;
}
