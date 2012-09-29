#include "sm_aboutdialog.h"
#include "ui_sm_aboutdialog.h"

SM_AboutDialog::SM_AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SM_AboutDialog)
{
    ui->setupUi(this);
}

SM_AboutDialog::~SM_AboutDialog()
{
    delete ui;
}
