#include "sm_infodockwidget.h"
#include "ui_sm_infodockwidget.h"
#include "subnet_v4.h"
#include "subnet_v6.h"

SM_InfoDockWidget::SM_InfoDockWidget(Subnet *sn, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SM_InfoDockWidget)
{
    ui->setupUi(this);
}

SM_InfoDockWidget::~SM_InfoDockWidget()
{
    delete ui;
}

void SM_InfoDockWidget::setSubnet(Subnet *sn)
{
    if (sn->isV4()) {
        Subnet_v4 *momnet = (Subnet_v4*)sn;
    } else {
        Subnet_v4 *momnet = (Subnet_v4*)sn;
    };
}

