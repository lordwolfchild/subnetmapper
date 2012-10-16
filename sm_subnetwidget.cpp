#include "sm_subnetwidget.h"
#include <QPainter>
#include <QPaintEvent>

SM_SubnetWidget::SM_SubnetWidget(QWidget *parent) :
    QWidget(parent)
{
    setModel(NULL);
    setSelectionModel(NULL);
    setMinimumSize(400,400);
}

void SM_SubnetWidget::setModel(SM_DataModel *newmodel)
{
    model=newmodel;
}

void SM_SubnetWidget::setSelectionModel(QItemSelectionModel *newselectionmodel)
{
    selectionModel=newselectionmodel;
}


void SM_SubnetWidget::paintEvent(QPaintEvent *event)
{

    model->sortData();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(),Qt::white);
    painter.setPen(Qt::black);

    int itemCount=model->rowCount();

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
    for (int i=0;i<model->rowCount();i++) {
        if (model->data(model->index(i,1),Qt::UserRole)=="IPv4") {
            subnetsV4.insert(i);
            ipv4cache[((((Subnet_v4*)(model->getSubnet(i)))->getIP())&(~((quint32)255)))].append(i);
            countV4++;
        } else {
            subnetsV6.insert(i);
            QString momip = (model->data(model->index(i,2),Qt::UserRole)).toString();
            ipv6cache[momip.left(34)].append(i);
            countV6++;
        };
    };

    // Store the original state of the painter...
    painter.save();

    // define some constants for drawing calulations TODO: Move them to options file/dialog
    uint general_margin = 20;

    uint text_offset = 4;

    uint x_offset = 80;
    uint y_offset = 20;

    uint x_width = 600;

    uint line_height = 20;

    QFont newFont = painter.font();
    newFont.setPixelSize(line_height-(text_offset*2));
    painter.setFont(newFont);

    uint y_internetwork_spacer = 50;
    uint y_interversion_spacer = 100;

    // a helper to make the point calculations more readable. holds the offset for each block of the graphs.
    uint y_local_offset = general_margin + y_offset;

    // 1.1 Draw legend for IPv4
    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    y_local_offset = general_margin + y_offset + (line_height*(subnetsV4.count()+1)) + y_internetwork_spacer;

    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV4.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    // 1.2 Draw IPv4 subnets

    for (int line=0;line<ipv4cache.keys().count();line++) {
        quint32 index=ipv4cache.keys()[line];
        for (int net=0;net<ipv4cache[index].count();net++) {
            Subnet_v4 *momNet=(Subnet_v4*)model->getSubnet(ipv4cache[index].at(net));

            uint y_block1_offset = general_margin + y_offset;
            uint y_block2_offset = general_margin + y_offset + (line_height*(subnetsV4.count()+1)) + y_internetwork_spacer;

            // We have our subnet, now we need to decide, where we put it. To achieve this, we have to analyze its size and its position.

            quint32 start_position = (((quint32)255)&(momNet->getIP()));
            quint32 end_position = (((quint32)255)&(momNet->getBroadcast()));
            quint32 size = momNet->getSize();

            painter.setBrush( QBrush( momNet->getColor() ));

            if (start_position<128)  {
                if (end_position>128) {
                    painter.drawRect(general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),x_width,line_height);
                    painter.drawText(text_offset+general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),x_width-(text_offset*2),line_height,Qt::AlignVCenter,momNet->toString());
                    painter.drawRect(general_margin+x_offset,y_block2_offset+(line_height*(line+1)),x_width,line_height);
                    painter.drawText(text_offset+general_margin+x_offset,y_block2_offset+(line_height*(line+1)),x_width-(text_offset),line_height,Qt::AlignVCenter,momNet->toString());
                    if (momNet->getSize()>256) painter.drawText(text_offset+general_margin+x_offset,y_block2_offset+(line_height*(line+1)),x_width-(text_offset*2),line_height,Qt::AlignVCenter|Qt::AlignRight,tr("+"));
                } else {
                    painter.drawRect(general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),(((float)x_width/128)*end_position),line_height);
                    painter.drawText(text_offset+general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),(((float)x_width/128)*end_position),line_height,Qt::AlignVCenter,momNet->toString());
                }
            }
            else {
                painter.drawRect(general_margin+x_offset+(((float)x_width/128)*(start_position-128)),y_block2_offset+(line_height*(line+1)),(((float)x_width/128)*size),line_height);
                painter.drawText(text_offset+general_margin+x_offset+(((float)x_width/128)*(start_position-128)),y_block2_offset+(line_height*(line+1)),(((float)x_width/128)*size),line_height,Qt::AlignVCenter,momNet->toString());
            }
            painter.setBrush( Qt::NoBrush );
        }
    }

    // 1.3 Draw Extras for IPv4

    // stores the offset which the ipv6 block always has through the ipv4 block
    uint y_offset_ipv6 = general_margin + ((y_offset + (line_height*(subnetsV4.count()+1)))*2) + y_internetwork_spacer + y_interversion_spacer;

    y_local_offset = y_offset_ipv6 + y_offset;

    // 2.1 Draw legend for IPv6

    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    y_local_offset = y_offset_ipv6 + y_offset + (line_height*(subnetsV6.count()+1)) + y_internetwork_spacer;

    painter.drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(subnetsV6.count()+1))));
    painter.drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    // 2.2 Draw IPv6 subnets

    // 2.3 Draw Extras for IPv6

    // resize the widget to the needed size
    resize((2*general_margin)+(2*x_offset)+x_width, y_local_offset+(line_height*(subnetsV6.count()+1))+y_offset+general_margin);

    // Put everything back in the state we found it in (Is this even necessary?!).
    painter.restore();

}

void SM_SubnetWidget::dataHasChanged()
{
    qDebug("Data has changed!");
};
