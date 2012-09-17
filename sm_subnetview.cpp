#include "sm_subnetview.h"
#include <QScrollBar>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>

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

void SM_SubnetView::paintEvent(QPaintEvent *event)
{
    QItemSelectionModel *selections = selectionModel();
    QStyleOptionViewItem option = viewOptions();
    QStyle::State state = option.state;

    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));
    QPen textPen(option.palette.color(QPalette::Text));
    QPen highlightedPen(option.palette.color(QPalette::HighlightedText));

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(),background);
    painter.setPen(foreground);

    int itemCount=model()->rowCount();

    int countV6=0;
    int countV4=0;

    // caches the row indices of v4/v6 Subnets seperately.
    QSet<uint> subnetsV4,subnetsV6;

    // This will store a structure of prefixes that are to be rendered on the same line. Something like:
    // ipv4cache[Subnet::String2IP("192.168.0.0")].append(8);
    // ipv4cache[Subnet::String2IP("192.168.0.0")].append(10);
    // ... if Index #8 and #10 are in the same subnet (and henceforth in the same line). Then we only need
    // to iterate this structure to ease up the whole rendering process.
    QHash< quint32, QList<uint> > ipv4cache;

    // We would do the same for IPv6, but as we need longer prefixes here, we use string represantations instead.
    // As we would have to do some serious magic here to speed up things we just hope that string handling will not
    // take too much cpu time... *fingerscrossed*. That method has the advantange, that we do not need the
    // Subnet Helpers for conversion all the time. ;)
    QHash< QString, QList<uint> > ipv6cache;


    // Iterate the Model and store the data we need to prepare our little drawing.
    for (int i=0;i<model()->rowCount();i++) {
        if (model()->data(model()->index(i,0),Qt::UserRole)=="IPv4") {
            subnetsV4.insert(i);
            ipv4cache[((model()->data(model()->index(i,1),Qt::UserRole)).toUInt()&(~((quint32)255)))].append(i);
            countV4++;
        } else {
            subnetsV6.insert(i);
            QString momip = (model()->data(model()->index(i,1),Qt::UserRole)).toString();
            ipv6cache[momip.left(34)].append(i);
            countV6++;
        };
    };

    // Store the original state of the painter...
    painter.save();

    // TODO: Draw stuff for real

    // Draw legend for IPv4

    // Draw IPv4 subnets

    // Draw Extras for IPv4

    // Draw legend for IPv6

    // Draw IPv6 subnets

    // Draw Extras for IPv6

    // Put everything back in the state we found it in (Is this even necessary?!).
    painter.restore();

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


