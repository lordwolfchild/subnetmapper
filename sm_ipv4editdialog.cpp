#include "sm_ipv4editdialog.h"
#include "ui_sm_ipv4editdialog.h"

sm_IPv4EditDialog::sm_IPv4EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sm_IPv4EditDialog)
{
    ui->setupUi(this);
}

sm_IPv4EditDialog::~sm_IPv4EditDialog()
{
    delete ui;
}
