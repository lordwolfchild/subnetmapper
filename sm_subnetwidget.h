#ifndef SM_SUBNETWIDGET_H
#define SM_SUBNETWIDGET_H

#include <QWidget>
#include <QItemSelectionModel>
#include <sm_datamodel.h>
#include <QTimer>
#include <QHostInfo>

class SM_SubnetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SM_SubnetWidget(QWidget *parent = 0);
    ~SM_SubnetWidget();

    void setModel(SM_DataModel *newmodel);
    void setSelectionModel(QItemSelectionModel *newselectionmodel);

    bool searchHosts( QString name );

    void renderprt(QPainter *painter,int width,int height);

    void paintJob(QPainter *painter, QRect paintArea);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event );
    void mouseDoubleClickEvent(QMouseEvent *event );
    void clearCache();

signals:

public slots:

    void dataHasChanged();
    void selectionChangedInTable(const QModelIndex & current, const QModelIndex & previous);
    void selAnimTimerTriggered();

    void editCurrentSubnet();

    void xWidthPlus();
    void xWidthMinus();
    void line_heightPlus();
    void line_heightMinus();
    void upscale();

private:
    SM_DataModel *model;
    QItemSelectionModel *selectionModel;

    QList<QRect*> rectCache1_v4;
    QList<QRect*> rectCache2_v4;
    QList<QRect*> rectCache1_v6;
    QList<QRect*> rectCache2_v6;

    QList<QRect*> selectionCache;

    unsigned short selAnimState;
    QTimer *selAnimTimer;

    uint x_width;

    uint line_height;

    uint general_margin;

    QHostInfo searchedHosts;

};

#endif // SM_SUBNETWIDGET_H
