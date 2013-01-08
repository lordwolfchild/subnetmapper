#include "sm_infodockwidget.h"
#include "ui_sm_infodockwidget.h"

SM_InfoDockWidget::SM_InfoDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SM_InfoDockWidget)
{
    ui->setupUi(this);
}

SM_InfoDockWidget::~SM_InfoDockWidget()
{
    delete ui;
}
