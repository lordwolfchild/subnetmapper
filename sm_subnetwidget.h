#ifndef SM_SUBNETWIDGET_H
#define SM_SUBNETWIDGET_H

#include <QWidget>
#include <sm_datamodel.h>

class SM_SubnetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SM_SubnetWidget(QWidget *parent = 0);

    void setModel(SM_DataModel *newmodel);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    SM_DataModel *model;

};

#endif // SM_SUBNETWIDGET_H
