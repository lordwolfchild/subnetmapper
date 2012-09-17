#include "sm_subnetview.h"
#include <QScrollBar>

SM_SubnetView::SM_SubnetView(QWidget *parent) :
    QAbstractItemView(parent)
{
}

QRect SM_SubnetView::visualRect(const QModelIndex &index) const
{
    return QRect(10,10,100,100);
}

void SM_SubnetView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
}

QModelIndex SM_SubnetView::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

QModelIndex SM_SubnetView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    return QModelIndex();
}

int SM_SubnetView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int SM_SubnetView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool SM_SubnetView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

void SM_SubnetView::setSelection(const QRect &, QItemSelectionModel::SelectionFlags command)
{
}

QRegion SM_SubnetView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion(10,10,100,100);
}
