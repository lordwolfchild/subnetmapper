#include "sm_infodockwidget.h"
#include "ui_sm_infodockwidget.h"
#include "subnet_v4.h"
#include "subnet_v6.h"
#include "mainwindow.h"

SM_InfoDockWidget::SM_InfoDockWidget(Subnet *sn, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SM_InfoDockWidget)
{
    ui->setupUi(this);
    referencedSubnet=sn;

    connect(ui->sn_notes,SIGNAL(textChanged()),this,SLOT(notesEdited()));
    connect(ui->editButton,SIGNAL(clicked()),(MainWindow*)parentWidget(),SLOT(editCurrentSubnet()));
}

SM_InfoDockWidget::~SM_InfoDockWidget()
{
    delete ui;
}

void SM_InfoDockWidget::setSubnet(Subnet *sn)
{
    referencedSubnet=sn;
    updateSubnet();
}

void SM_InfoDockWidget::updateSubnet()
{
    if (referencedSubnet==0) {
        ui->sn_addr->setText("n/a");
        ui->sn_bc->setText("n/a");
        ui->sn_desc->setText("n/a");
        ui->sn_first->setText("n/a");
        ui->sn_last->setText("n/a");
        ui->sn_name->setText("n/a");
        ui->sn_nm->setText("n/a");
        ui->sn_notes->setText("n/a");
        ui->sn_size->setText("n/a");
        ui->sn_wc->setText("n/a");
        ui->sn_ipver->setText("X");
        ui->sn_color->setStyleSheet("");

        ui->sn_notes->setReadOnly(true);

        // We did our job for referencedSubnet=0. Anything else would produce SEGV anyways...
        return;
    };

    if (referencedSubnet->isV4()) {
        Subnet_v4 *momnet = (Subnet_v4*)referencedSubnet;

        quint32 wildcard=momnet->getWildcard();
        quint32 first = momnet->getFirstUsableIP();
        quint32 last = momnet->getLastUsableIP();
        quint32 broadcast = momnet->getBroadcast();
        quint32 netmask = momnet->getNM();

        ui->sn_addr->setText(momnet->toString());
        ui->sn_bc->setText(momnet->IP2String(broadcast));
        ui->sn_desc->setText(momnet->getDescription());
        ui->sn_first->setText(momnet->IP2String(first));
        ui->sn_last->setText(momnet->IP2String(last));
        ui->sn_name->setText(momnet->getIdentifier());
        ui->sn_nm->setText(momnet->IP2String(netmask));
        ui->sn_notes->setText(momnet->getNotes());
        ui->sn_size->setText(QString::number(momnet->getSize()));
        ui->sn_wc->setText(momnet->IP2String(wildcard));
        ui->sn_ipver->setText("4");
        ui->sn_color->setStyleSheet("QLabel { background:"
                                    + momnet->getColor().name()
                                    + "; } ");

        ui->sn_notes->setReadOnly(false);


    } else {
        Subnet_v6 *momnet = (Subnet_v6*)referencedSubnet;

        QPair<quint64,quint64>  wildcard=momnet->getWildcard();
        QPair<quint64,quint64>  netmask=momnet->getNM();
        QPair<quint64,quint64>  first = momnet->getFirstUsableIP();
        QPair<quint64,quint64>  last = momnet->getLastUsableIP();
        QPair<quint64,quint64>  broadcast = momnet->getBroadcast();

        ui->sn_addr->setText(momnet->toString());
        ui->sn_bc->setText(momnet->reduceIP(momnet->IP2String(broadcast)));
        ui->sn_desc->setText(momnet->getDescription());
        ui->sn_first->setText(momnet->reduceIP(momnet->IP2String(first)));
        ui->sn_last->setText(momnet->reduceIP(momnet->IP2String(last)));
        ui->sn_name->setText(momnet->getIdentifier());
        ui->sn_nm->setText(momnet->reduceIP(momnet->IP2String(netmask)));
        ui->sn_notes->setText(momnet->getNotes());
        ui->sn_size->setText(QString::number(wildcard.second));
        ui->sn_wc->setText(momnet->reduceIP(momnet->IP2String(wildcard)));
        ui->sn_ipver->setText("6");
        ui->sn_color->setStyleSheet("QLabel { background:"
                                    + momnet->getColor().name()
                                    + "; } ");

        ui->sn_notes->setReadOnly(false);

    };
}

void SM_InfoDockWidget::notesEdited()
{
    QString notes = ui->sn_notes->toPlainText();
    if (referencedSubnet) referencedSubnet->setNotes(notes);
}

