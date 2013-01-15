#include "sm_subnetwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include "sm_ipv4editdialog.h"
#include "sm_ipv6editdialog.h"
#include <QMessageBox>
#include <QTimer>
#include <QtNetwork/QHostInfo>
#include <QIcon>
#include <QSvgRenderer>
#include <QSettings>
#include "mainwindow.h"

SM_SubnetWidget::SM_SubnetWidget(QWidget *parent) :
    QWidget(parent)
{
    QSettings settings;

    setModel(NULL);
    setSelectionModel(NULL);
    setMinimumSize(400,400);
    selAnimState=0;

    selAnimTimer = new QTimer(this);
    connect(selAnimTimer, SIGNAL(timeout()), this, SLOT(selAnimTimerTriggered()));
    selAnimTimer->start(50);

    x_width = settings.value("ipv4widget/x_width",600).toUInt();
    line_height = settings.value("ipv4widget/line_height",20).toUInt();
    general_margin = settings.value("ipv4widget/general_margin",30).toUInt();

}

void SM_SubnetWidget::setModel(SM_DataModel *newmodel)
{
    model=newmodel;
}

void SM_SubnetWidget::setSelectionModel(QItemSelectionModel *newselectionmodel)
{
    selectionModel=newselectionmodel;
    if (selectionModel) connect(selectionModel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(selectionChangedInTable(QModelIndex,QModelIndex)));
}

bool SM_SubnetWidget::searchHosts(QString name)
{
  // if the search field has no current input... (i.e. the clear button has been pressed)
  if (name.isEmpty())
  {
    // ... clear the lookup data from before ...
    searchedHosts=QHostInfo();
    // ... and bail out. We do not need to do more regarding this matter.
    return true;
  }

  // search for the search field input
  searchedHosts = QHostInfo::fromName(name);

  // now this object knows about the searched hosts. The referring Pins will be placed in the next redraw:
  repaint();

  if (searchedHosts.error()==QHostInfo::NoError)
  {
      qDebug("valid and found %u addresses.",searchedHosts.addresses().count());
      return true;
  } else {
      qDebug("Error: %s",qPrintable(searchedHosts.errorString()));
      return false;
  };

}

void SM_SubnetWidget::paintJob(QPainter *painter, QRect paintArea)
{
    painter->fillRect(paintArea,Qt::white);

    // prepare our model and metadata for drawing of the map
    model->sortData();
    clearCache();

    int countV6=0;
    int countV4=0;

    // caches the row indices of v4/v6 Subnets seperately.
    QSet<uint> subnetsV4,subnetsV6;

    // This will store a structure of prefixes that are to be rendered on the same line. Something like:
    // ipv4cache[Subnet::String2IP("192.168.0.0")].append(8);
    // ipv4cache[Subnet::String2IP("192.168.0.0")].append(10);
    // ... if Index #8 and #10 are in the same subnet (and henceforth in the same line). Then we only need
    // to iterate this structure to ease up the whole rendering process.
    QMap< quint32, QList<uint> > ipv4cache;

    // We would do the same for IPv6, but as we need longer prefixes here, we use string representations instead.
    // As we would have to do some serious magic here to speed up things we just hope that string handling will not
    // take too much cpu time... *fingerscrossed*. That method has the advantange, that we do not need the
    // Subnet Helpers for conversion all the time. ;)
    QMap< QString, QList<uint> > ipv6cache;

    // stores a list of coordinates for placement of the host pins, if a search is available
    QList<QPoint*> pinList;

    // stores a list of coordinates for placement of the notes icon, if notes are featured in a subnet
    QList<QPoint*> notesList;

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


    // define some helpers for drawing calculations

    uint text_offset = x_width/150;

    uint x_offset = x_width/5;
    uint y_offset = line_height;

    QFont newFont = painter->font();
    newFont.setPixelSize(line_height-(text_offset*2));
    painter->setFont(newFont);

    QPen grayDotted= QPen( Qt::gray,1,Qt::DotLine);
    QPen grayDashed= QPen( Qt::gray,1,Qt::DashLine);

    uint y_internetwork_spacer = 2*line_height;
    //uint y_interversion_spacer = 5*line_height;

    // a helper to make the point calculations more readable. holds the offset for each block of the graphs.
    uint y_local_offset = general_margin + y_offset;

    // 1.1 Draw legend for IPv4
    painter->drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(ipv4cache.count()+1))));
    painter->setPen(grayDotted);
    painter->drawLine(QPoint(general_margin+x_offset+(x_width/4), y_local_offset),QPoint(general_margin+x_offset+(x_width/4),general_margin+(y_local_offset+(line_height*(ipv4cache.count()+1)))*2));
    painter->setPen(grayDashed);
    painter->drawLine(QPoint(general_margin+x_offset+(x_width/2), y_local_offset),QPoint(general_margin+x_offset+(x_width/2),general_margin+(y_local_offset+(line_height*(ipv4cache.count()+1)))*2));
    painter->setPen(grayDotted);
    painter->drawLine(QPoint(general_margin+x_offset+((x_width/4)*3), y_local_offset),QPoint(general_margin+x_offset+((x_width/4)*3),general_margin+(y_local_offset+(line_height*(ipv4cache.count()+1)))*2));
    painter->setPen(Qt::black);
    painter->drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(ipv4cache.count()+1))));
    painter->drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));
    painter->setPen(Qt::gray);
    painter->drawText(general_margin+x_offset+((x_width/4)*0)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".0");
    painter->drawText(general_margin+x_offset+((x_width/4)*1)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".32");
    painter->drawText(general_margin+x_offset+((x_width/4)*2)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".64");
    painter->drawText(general_margin+x_offset+((x_width/4)*3)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".96");
    painter->drawText(general_margin+x_offset+((x_width/4)*4)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".127");
    painter->setPen(Qt::black);

    y_local_offset = general_margin + y_offset + (line_height*(ipv4cache.count()+1)) + y_internetwork_spacer;

    painter->drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(ipv4cache.count()+1))));
    painter->drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(ipv4cache.count()+1))));
    painter->drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));
    painter->setPen(Qt::gray);
    painter->drawText(general_margin+x_offset+((x_width/4)*0)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".128");
    painter->drawText(general_margin+x_offset+((x_width/4)*1)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".160");
    painter->drawText(general_margin+x_offset+((x_width/4)*2)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".192");
    painter->drawText(general_margin+x_offset+((x_width/4)*3)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".224");
    painter->drawText(general_margin+x_offset+((x_width/4)*4)+text_offset,y_local_offset,(x_width/8)-(2*text_offset),line_height,Qt::AlignVCenter,".255");
    painter->setPen(Qt::black);

    // 1.2 Draw IPv4 subnets

    for (int line=0;line<ipv4cache.keys().count();line++) {
        quint32 index=ipv4cache.keys()[line];

        uint y_block1_offset = general_margin + y_offset;
        uint y_block2_offset = general_margin + y_offset + (line_height*(ipv4cache.count()+1)) + y_internetwork_spacer;

        // drawing the net legend (/24 Network Address on the left side of the map)
        QString legend_str=Subnet_v4::IP2String(ipv4cache.keys()[line]);
        legend_str=legend_str.left(legend_str.lastIndexOf('.'))+".*";
        painter->drawText(general_margin+text_offset,y_block1_offset+(line_height*(line+1)),x_offset-(2*text_offset),line_height,Qt::AlignVCenter,legend_str);
        painter->drawText(general_margin+text_offset,y_block2_offset+(line_height*(line+1)),x_offset-(2*text_offset),line_height,Qt::AlignVCenter,legend_str);

        for (int net=0;net<ipv4cache[index].count();net++) {
            Subnet_v4 *momNet=(Subnet_v4*)model->getSubnet(ipv4cache[index].at(net));

            // We have our subnet, now we need to decide, where we put it. To achieve this, we have to analyze its size and its position.

            quint32 start_position = (((quint32)255)&(momNet->getIP()));
            quint32 end_position = (((quint32)255)&(momNet->getBroadcast()));
            quint32 size = momNet->getSize();

            painter->setBrush( QBrush( momNet->getColor() ));

            qreal r,g,b;
            QColor invColor;

            momNet->getColor().getRgbF(&r,&g,&b);
            invColor.setRgbF(1.0-r,1.0-g,1.0-b);

            if (start_position<128)  {
                if (end_position>128) {

                    // two blocks, size 255

                    QRect* momRect1= new QRect(general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),x_width,line_height);
                    QRect* momRect2= new QRect(general_margin+x_offset,y_block2_offset+(line_height*(line+1)),x_width,line_height);
                    rectCache1_v4.append(momRect1);
                    rectCache2_v4.append(momRect2);

                    painter->drawRect(*momRect1);
                    painter->setPen(invColor);
                    painter->drawText(text_offset+general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),x_width-(text_offset*2),line_height,Qt::AlignVCenter,momNet->getIdentifier());
                    painter->setPen(Qt::black);
                    painter->drawRect(*momRect2);
                    painter->setPen(invColor);
                    painter->drawText(text_offset+general_margin+x_offset,y_block2_offset+(line_height*(line+1)),x_width-(text_offset),line_height,Qt::AlignVCenter,momNet->getIdentifier());
                    painter->setPen(Qt::black);
                    if (momNet->getSize()>256) painter->drawText(text_offset+general_margin+x_offset,y_block2_offset+(line_height*(line+1)),x_width-(text_offset*2),line_height,Qt::AlignVCenter|Qt::AlignRight,tr("+"));

                    if (momNet->getSelected()) {
                        selectionCache.append(momRect1);
                        selectionCache.append(momRect2);
                    }


                } else {
                    // one block, in between 0-127

                    QRect* momRect1= new QRect(general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),(((float)x_width/128)*size),line_height);
                    QRect* momRect2= new QRect(0,0,0,0);
                    rectCache1_v4.append(momRect1);
                    rectCache2_v4.append(momRect2);

                    painter->drawRect(*momRect1);
                    painter->setPen(invColor);
                    painter->drawText(text_offset+general_margin+x_offset+(((float)x_width/128)*start_position),y_block1_offset+(line_height*(line+1)),(((float)x_width/128)*size)-(text_offset*2),line_height,Qt::AlignVCenter,momNet->getIdentifier());
                    painter->setPen(Qt::black);

                    if (momNet->getSelected()) {
                        selectionCache.append(momRect1);
                    }
                }
            }
            else {
                // one block, in between 128-255

                QRect* momRect1= new QRect(0,0,0,0);
                QRect* momRect2= new QRect(general_margin+x_offset+(((float)x_width/128)*(start_position-128)),y_block2_offset+(line_height*(line+1)),(((float)x_width/128)*size),line_height);
                rectCache1_v4.append(momRect1);
                rectCache2_v4.append(momRect2);

                painter->drawRect(*momRect2);
                painter->setPen(invColor);
                painter->drawText(text_offset+general_margin+x_offset+(((float)x_width/128)*(start_position-128)),y_block2_offset+(line_height*(line+1)),(((float)x_width/128)*size)-(text_offset*2),line_height,Qt::AlignVCenter,momNet->getIdentifier());
                painter->setPen(Qt::black);

                if (momNet->getSelected()) {
                    selectionCache.append(momRect2);
                }
            }
            painter->setBrush( Qt::NoBrush );

            // check subnet for searched hosts
            for (int i=0;i<searchedHosts.addresses().count();i++)
            {
              quint32 addr=searchedHosts.addresses().at(i).toIPv4Address();
              if (momNet->containsHost(addr)) {
                quint32 pinPos=(((quint32)255)&(addr));
                QPoint *pinPoint=  new QPoint();
                if (pinPos<128) {
                  // upper row
                  pinPoint->setX(general_margin+x_offset+(((float)x_width/128)*pinPos));
                  pinPoint->setY(y_block1_offset+(line_height*(line+1)));
                } else {
                  // lower row
                  pinPoint->setX(general_margin+x_offset+(((float)x_width/128)*(pinPos-128)));
                  pinPoint->setY(y_block2_offset+(line_height*(line+1)));
                };
                pinList.append(pinPoint);
              }
            }

            if ((momNet->getNotes()!="n/a")&(!(momNet->getNotes().isEmpty()))) {
                quint32 notesPos=end_position+1;
                QPoint *notesPoint= new QPoint();
                if (notesPos<=128) {
                  // upper row
                  notesPoint->setX((general_margin+x_offset+(((float)x_width/128)*notesPos))-line_height);
                  notesPoint->setY(y_block1_offset+(line_height*(line+1)));
                  notesList.append(notesPoint);
                } else {
                  // lower row
                  notesPoint->setX((general_margin+x_offset+(((float)x_width/128)*(notesPos-128)))-line_height);
                  notesPoint->setY(y_block1_offset+(line_height*(line+1)));
                  notesList.append(notesPoint);
                  notesPoint = new QPoint();
                  notesPoint->setX((general_margin+x_offset+(((float)x_width/128)*(notesPos-128)))-line_height);
                  notesPoint->setY(y_block2_offset+(line_height*(line+1)));
                  notesList.append(notesPoint);
                };

            }
        }
    }


    // 1.3 Draw Extras for IPv4

    /* OK, I killed the IPv6 drawing for now. Thats something for the next minor release... :P (or if I actually need it myself)

    // stores the offset which the ipv6 block always has through the ipv4 block
    uint y_offset_ipv6 = general_margin + ((y_offset + (line_height*(ipv4cache.count()+1)))*2) + y_internetwork_spacer + y_interversion_spacer;

    y_local_offset = y_offset_ipv6 + y_offset;

    // 2.1 Draw legend for IPv6

    painter->drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(ipv6cache.count()+1))));
    painter->drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(ipv6cache.count()+1))));
    painter->drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    y_local_offset = y_offset_ipv6 + y_offset + (line_height*(ipv6cache.count()+1)) + y_internetwork_spacer;

    painter->drawLine(QPoint(general_margin+x_offset,y_local_offset),QPoint(general_margin+x_offset, y_local_offset+(line_height*(ipv6cache.count()+1))));
    painter->drawLine(QPoint(general_margin+x_offset+x_width, y_local_offset),QPoint(general_margin+x_offset+x_width,y_local_offset+(line_height*(ipv6cache.count()+1))));
    painter->drawLine(QPoint(general_margin,y_local_offset+line_height),QPoint(general_margin+x_offset+x_width,y_local_offset+line_height));

    // 2.2 Draw IPv6 subnets

    // 2.3 Draw Extras for IPv6

    */

    // resize the widget to the needed size WITHOUT IPv6 networks
    resize((2*general_margin)+(2*x_offset)+x_width, y_local_offset+(line_height*(ipv4cache.count()+1))+y_offset+general_margin);

    // 3 Draw Selection Boxes
    QPen selectedPenUp = QPen( Qt::white,1.8,Qt::DashLine);
    QPen selectedPenDown = QPen( Qt::blue,3.4,Qt::SolidLine);

    for (int selIndex=0;selIndex<selectionCache.count();selIndex++) {
        painter->setBrush(Qt::transparent);
        painter->setPen(selectedPenDown);
        painter->drawRect(*(selectionCache.at(selIndex)));
        selectedPenUp.setDashOffset((qreal)selAnimState);
        painter->setPen(selectedPenUp);
        painter->drawRect(*(selectionCache.at(selIndex)));
        painter->setPen(Qt::black);
    }


    // 4 Draw Notes Marker
    // the SVG Renderer we use for drawing our info-sign
    QSvgRenderer notesRenderer(tr(":info.svg"));

    // iterate the found host coordinates cache
    for (int i=0;i<notesList.count();i++) {
      QRectF momRect(notesList.at(i)->x(),notesList.at(i)->y(),line_height,line_height);

      // now draw it into the constructed rectangle
      notesRenderer.render(painter, momRect);
    }

    // 5 Draw Search Pins
    // the SVG Renderer we use for drawing our host-pin
    QSvgRenderer momRenderer(tr(":pin.svg"));

    // iterate the found host coordinates cache
    for (int i=0;i<pinList.count();i++) {
      // 1.3 is the aspect ratio of the pin graphic... Hey, stop staring. constant values guarantee quick calcs. ;) The rest are the offsets for the
      // needle tip position.
      QRectF momRect(pinList.at(i)->x()-(line_height*0.1),pinList.at(i)->y()-((line_height*1.3)/2.0),line_height,line_height*1.3);

      // now draw it into the constructed rectangle
      momRenderer.render(painter, momRect);
    }

    // clear the pinList cache and delete it's contents (Ok, the other way around, so we do not loose the references to our cached objects)
    for (int i=0;i<pinList.count();i++) {
      delete pinList.at(i);
    };
    pinList.clear();

    // clear the pinList cache and delete it's contents (Ok, the other way around, so we do not loose the references to our cached objects)
    for (int i=0;i<notesList.count();i++) {
      delete notesList.at(i);
    };
    notesList.clear();

}


void SM_SubnetWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    // Store the original state of the painter...
    painter.save();

    paintJob(&painter,event->rect());

    // Put everything back in the state we found it in (Is this even necessary?!).
    painter.restore();

}

void SM_SubnetWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {

        bool foundClickedSubnet=false;
        for (int i=0;i<rectCache1_v4.count();i++) {
            if ((rectCache1_v4.at(i)->contains(event->x(),event->y()))|
                    (rectCache2_v4.at(i)->contains(event->x(),event->y())))
            {
                (model->getSubnet(i))->setSelected(true);
                selectionModel->setCurrentIndex(model->index(i,0),QItemSelectionModel::Select|QItemSelectionModel::Rows|QItemSelectionModel::Clear);
                foundClickedSubnet=true;
            } else (model->getSubnet(i))->setSelected(false);
        }

        for (int i=0;i<rectCache1_v6.count();i++) {
            if ((rectCache1_v6.at(i)->contains(event->x(),event->y()))|
                    (rectCache2_v6.at(i)->contains(event->x(),event->y())))
            {
                (model->getSubnet(i))->setSelected(true);
                selectionModel->setCurrentIndex(model->index(i,0),QItemSelectionModel::Select|QItemSelectionModel::Rows|QItemSelectionModel::Clear);
                foundClickedSubnet=true;
            } else (model->getSubnet(i))->setSelected(false);
        }
        if (!foundClickedSubnet) selectionModel->clearSelection();

        repaint();
    }

    QWidget::mousePressEvent(event);
}

void SM_SubnetWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    editCurrentSubnet();
    QWidget::mouseDoubleClickEvent(event);
}

void SM_SubnetWidget::clearCache()
{
    for (int i=0;i<rectCache1_v4.count();i++) {
        delete rectCache1_v4.at(i);
    }
    rectCache1_v4.clear();

    for (int i=0;i<rectCache2_v4.count();i++) {
        delete rectCache2_v4.at(i);
    }
    rectCache2_v4.clear();

    for (int i=0;i<rectCache1_v6.count();i++) {
        delete rectCache1_v6.at(i);
    }
    rectCache1_v6.clear();

    for (int i=0;i<rectCache2_v6.count();i++) {
        delete rectCache2_v6.at(i);
    }
    rectCache2_v6.clear();

    // this ones members we do not need to delete - all the Rects have already been deleted in the statements before through their respective cache cleanups
    selectionCache.clear();
}

void SM_SubnetWidget::selAnimTimerTriggered()
{
    selAnimState++;
    if (selAnimState>5) selAnimState=0;
    repaint();
}

void SM_SubnetWidget::editCurrentSubnet()
{
    if ((selectionModel->currentIndex().isValid())&(selectionModel->hasSelection())) {

        Subnet *momsubnet=model->getSubnet(selectionModel->currentIndex().row());

        if (momsubnet) {
            if (momsubnet->isV4()) {
                // edit IPv4
                sm_IPv4EditDialog editor(this);
                Subnet_v4 *momsubnet_v4 = (Subnet_v4*)momsubnet;
                QColor momColor=momsubnet->getColor();

                editor.setDescription(momsubnet_v4->getDescription());
                editor.setIdentifier(momsubnet_v4->getIdentifier());
                editor.setIP(momsubnet_v4->getIP());
                editor.setNM(momsubnet_v4->getNM());
                editor.setColor(momColor);

                editor.setModal(true);
                if (editor.exec()==QDialog::Accepted) {

                    QString momdesc = editor.getDescription();
                    QString momid = editor.getIdentifier();
                    QColor momcolor = editor.getColor();
                    QString momIP = editor.getIP();
                    QString momNM = editor.getNM();

                    bool isNotOverlappingWithAnything=true;

                    for (int i=0;i<((SM_DataModel*)model)->rowCount();i++) {
                        if (((SM_DataModel*)model)->getSubnet(i)->isV4())
                            if (((Subnet_v4*)(((SM_DataModel*)model)->getSubnet(i)))->overlapsWith(*(momsubnet_v4)))
                                isNotOverlappingWithAnything=false;
                    }

                    if (isNotOverlappingWithAnything) {
                        momsubnet_v4->setIP(momIP);
                        momsubnet_v4->setNM(momNM);
                        momsubnet_v4->setDescription(momdesc);
                        momsubnet_v4->setIdentifier(momid);
                        momsubnet_v4->setColor(momcolor);
                        model->reset();
                        ((MainWindow*)window())->mapWasAltered();
                    } else {
                        QMessageBox msgBox;
                        msgBox.setText("The Subnet you specified overlaps with an existing subnet.");
                        msgBox.setIcon(QMessageBox::Critical);
                        msgBox.setDetailedText("The overlapping of subnets in a single map is not allowed in SubnetMapper. The author of this program cannot think of a situation in the real world where this could be an advisable situation. If you find yourself in a mess like this, please think it over, stuff like that always catches up with you at a later point in time, when you are actually least expecting it.");
                        msgBox.exec();
                    }

                    if (isNotOverlappingWithAnything) {
                        repaint();
                    };

                }



            } else {
                //edit IPv6
            }
        }

    }
}

void SM_SubnetWidget::xWidthPlus()
{
    QSettings settings;
    settings.setValue("ipv4widget/x_width",QVariant(x_width));

    x_width+=15;
    repaint();
}

void SM_SubnetWidget::xWidthMinus()
{
    QSettings settings;
    settings.setValue("ipv4widget/x_width",QVariant(x_width));

    x_width-=15;
    repaint();
}

void SM_SubnetWidget::line_heightPlus()
{
    QSettings settings;
    settings.setValue("ipv4widget/line_height",QVariant(line_height));

    line_height++;
    repaint();
}

void SM_SubnetWidget::line_heightMinus()
{
    QSettings settings;
    settings.setValue("ipv4widget/line_height",QVariant(line_height));

    line_height--;
    repaint();
}

void SM_SubnetWidget::upscale()
{
    QSettings settings;
    settings.setValue("ipv4widget/x_width",QVariant(x_width));
    settings.setValue("ipv4widget/line_height",QVariant(line_height));

    x_width=parentWidget()->width()/3*2;
    line_height=x_width/30;
    repaint();
}

void SM_SubnetWidget::dataHasChanged()
{
    this->repaint();
}

void SM_SubnetWidget::selectionChangedInTable(const QModelIndex &current, const QModelIndex &previous)
{
    if (previous.isValid()) model->getSubnet(previous.row())->setSelected(false);
    if (current.isValid()) model->getSubnet(current.row())->setSelected(true);

    repaint();
};

SM_SubnetWidget::~SM_SubnetWidget()
{
    QSettings settings;

    clearCache();
    selAnimTimer->stop();

    settings.setValue("ipv4widget/x_width",QVariant(x_width));
    settings.setValue("ipv4widget/line_height",QVariant(line_height));
    settings.setValue("ipv4widget/general_margin",QVariant(general_margin));

}
