#ifndef SM_INFODOCKWIDGET_H
#define SM_INFODOCKWIDGET_H

#include <QDockWidget>
#include "subnet.h"

namespace Ui {
class SM_InfoDockWidget;
}

class SM_InfoDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SM_InfoDockWidget(Subnet *sn = 0,QWidget *parent = 0);
    ~SM_InfoDockWidget();

    void setSubnet(Subnet *sn);
    void updateSubnet();

public slots:

    void notesEdited();


private:
    Ui::SM_InfoDockWidget *ui;
    Subnet *referencedSubnet;
};

#endif // SM_INFODOCKWIDGET_H
