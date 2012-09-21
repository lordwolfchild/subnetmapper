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

void SM_SubnetView::scrollContentsBy(int dx, int dy)
{
    viewport()->scroll(dx, dy);
}

void SM_SubnetView::updateGeometries()
{
    int totalSize=2000;

    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setRange(0, qMax(0, 2*totalSize - viewport()->width()));
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setRange(0, qMax(0, totalSize - viewport()->height()));

    viewport()->update();
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

    // We would do the same for IPv6, but as we need longer prefixes here, we use string representations instead.
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

    // define some constants for drawing calulations TODO: Move them to options file/dialog
    uint general_margin = 20;

    uint x_offset = 80;
    uint y_offset = 20;

    uint x_width = 600;

    uint line_height = 25;

    uint y_internetwork_spacer = 50;
    uint y_interversion_spacer = 100;

    // a helper to make the point calculations more readable. holds the offset for each block of the graphs.
    uint y_local_offset = general_margin + y_offset;

    // resize the widget to the needed size
    //this->resize((general_margin*2) + x_offset+x_width, (general_margin*2) + (y_offset*4) + ((((ipv4cache.count()+1)*line_height))*2) + ((((ipv6cache.count()+1)*line_height))*2) + y_internetwork_spacer + y_interversion_spacer);


    // 1.1 Draw legend for IPv4
    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    y_local_offset = general_margin + y_offset + (line_height*(subnetsV4.count()+1)) + y_internetwork_spacer;

    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    // 1.2 Draw IPv4 subnets

    // 1.3 Draw Extras for IPv4

    // stores the offset which the ipv6 block always has through the ipv4 block
    uint y_offset_ipv6 = general_margin + ((y_offset + (line_height*(subnetsV4.count()+1)))*2) + y_internetwork_spacer + y_interversion_spacer;

    y_local_offset = y_offset_ipv6 + y_offset;

    // 2.1 Draw legend for IPv6

    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    y_local_offset = y_offset_ipv6 + (y_offset*2) + ((line_height*(subnetsV6.count()+1))*2) + y_internetwork_spacer;

    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    // 2.2 Draw IPv6 subnets

    // 2.3 Draw Extras for IPv6

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


