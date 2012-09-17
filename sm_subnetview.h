#ifndef SM_SUBNETVIEW_H
#define SM_SUBNETVIEW_H

#include <QAbstractItemView>

class SM_SubnetView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit SM_SubnetView(QWidget *parent = 0);

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;

signals:

public slots:

protected:

    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                            Qt::KeyboardModifiers modifiers);

    int horizontalOffset() const;
    int verticalOffset() const;

    bool isIndexHidden(const QModelIndex &index) const;

    void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);

    QRegion visualRegionForSelection(const QItemSelection &selection) const;


};

#endif // SM_SUBNETVIEW_H
