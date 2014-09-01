/**
 *
 * This file is part of SubnetMapper.
 *
 * Copyright (C) 2008 "Thomas Thiel" <kv@wolfchild.de>
 *
 * SubnetMapper is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SubnetMapper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SubnetMapper.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mainwindow.h"
#include <iostream>
#include <QSplitter>
#include <QTableView>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QHeaderView>
#include <QFileDialog>
#include <QListView>
#include <QXmlStreamWriter>
#include <QXmlSimpleReader>
#include "sm_ipv4editdialog.h"
#include "sm_ipv6editdialog.h"
#include "sm_subnetwidget.h"
#include <QScrollArea>
#include <QMessageBox>
#include <QSlider>
#include "sm_aboutdialog.h"
#include <QScrollBar>
#include <QPushButton>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QLineEdit>
#include <QSettings>
#include <QApplication>
#include <QSize>
#include <QList>
#include <QHBoxLayout>
#include "sm_configdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QSettings settings;

    changedButNotSaved=false;

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QMenu *editMenu = new QMenu(tr("&Edit"), this);
    QMenu *viewMenu = new QMenu(tr("&View"), this);
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    recentDocsMenu = new QMenu(tr("&Recent Documents"), this);

    // prepare scaling control for ipv6 map
    ipv6Scale = new QSpinBox(this);
    ipv6Scale->setMaximumWidth(100);
    ipv6Scale->setMinimum(8);
    ipv6Scale->setMaximum(120);
    ipv6Scale->setSingleStep(8);
    int scalePreset=settings.value("mainwindow/ipv6scale",64).toUInt();
    if ((scalePreset%8==0)&(scalePreset<=120)&(scalePreset>=8)) ipv6Scale->setValue(settings.value("mainwindow/ipv6scale",64).toUInt());
    else ipv6Scale->setValue(settings.value("mainwindow/ipv6scale",64).toUInt());

    // Open File
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setIcon(QIcon(":/open.svg"));

    fileMenu->addMenu(recentDocsMenu);

    // Save File
    QAction *saveAction = fileMenu->addAction(tr("&Save As..."));
    saveAction->setShortcuts(QKeySequence::SaveAs);
    saveAction->setIcon(QIcon(":/save.svg"));

    // Print Map
    QAction *printAction = fileMenu->addAction(tr("&Print..."));
    printAction->setShortcut(QKeySequence(tr("Ctrl+P")));
    printAction->setIcon(QIcon(":/printer.svg"));

    // config Action
    QAction *configAction = fileMenu->addAction(tr("Configure..."));
    configAction->setShortcut((QKeySequence(tr("Ctrl+Shift+C"))));
    configAction->setIcon(QIcon(":/config.svg"));

    // Quit Program
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setIcon(QIcon(":/quit.svg"));

    // Add IPv4 Subnet
    QAction *addIPv4Action = editMenu->addAction(tr("Add IPv&4 Subnet"));
    addIPv4Action->setShortcut(QKeySequence(tr("Ctrl+N")));
    addIPv4Action->setIcon(QIcon(":/addipv4.svg"));

    // Add IPv6 Subnet
    QAction *addIPv6Action = editMenu->addAction(tr("Add IPv&6 Subnet"));
    addIPv6Action->setShortcut(QKeySequence(tr("Ctrl+Shift+N")));
    addIPv6Action->setIcon(QIcon(":/addipv6.svg"));

    // Add Edit Subnet
    QAction *editAction = editMenu->addAction(tr("Edit Subnet"));
    editAction->setShortcut(QKeySequence(tr("Ctrl+Shift+E")));
    editAction->setIcon(QIcon(":/edit.svg"));

    // Add Delete Subnet
    QAction *deleteAction = editMenu->addAction(tr("Delete Subnet"));
    deleteAction->setShortcut(QKeySequence(tr("Ctrl+Del")));
    deleteAction->setIcon(QIcon(":/delete.svg"));

    // Add Subnet Info
    QAction *showInfoAction = viewMenu->addAction(tr("Show Subnet Info"));
    showInfoAction->setShortcut(QKeySequence(tr("Ctrl+I")));
    showInfoAction->setIcon(QIcon(":/info.svg"));

    // About Dialog
    QAction *aboutAction = helpMenu->addAction(tr("&About SubnetMapper"));
    aboutAction->setIcon(QIcon(":/appicon.svg"));

    autoResizeOption = new QCheckBox("autoresize Map", this);
    if (settings.value("mainwindow/autoresize",0)==0)
        autoResizeOption->setChecked(false);
    else
        autoResizeOption->setChecked(true);

    toolbar = new QToolBar("Main Toolbar",this);
    ipv4toolbar = new QToolBar("IPv4 Toolbar",this);
    ipv6toolbar = new QToolBar("IPv6 Toolbar",this);

    setupModel();
    setupViews();

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(addIPv4Action, SIGNAL(triggered()), this, SLOT(addIPv4Subnet()));
    connect(addIPv6Action, SIGNAL(triggered()), this, SLOT(addIPv6Subnet()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(printAction,SIGNAL(triggered()),this,SLOT(printFile()));
    connect(modelBackend,SIGNAL(dataChanged()),this,SLOT(modelDataHasChanged()));
    connect(modelBackend,SIGNAL(data4Changed()),this,SLOT(modelDataHasChanged()));
    connect(modelBackend,SIGNAL(data6Changed()),this,SLOT(modelDataHasChanged()));
    connect(ipv6Scale,SIGNAL(valueChanged(int)),this,SLOT(updateIPv6Map()));
    connect(editAction, SIGNAL(triggered()),this,SLOT(editCurrentSubnet()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteCurrentSubnet()));
    connect(showInfoAction,SIGNAL(triggered()),this,SLOT(showInfoPane()));
    connect(configAction, SIGNAL(triggered()),this,SLOT(showConfigDialog()));

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    this->menuBar()->addMenu(viewMenu);
    this->menuBar()->addMenu(helpMenu);
    statusBar();

    this->addToolBar(Qt::TopToolBarArea,toolbar);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->addAction(openAction);
    toolbar->addAction(saveAction);
    toolbar->addAction(printAction);
    toolbar->addAction(addIPv4Action);
    toolbar->insertSeparator(addIPv4Action);
    toolbar->addAction(addIPv6Action);
    toolbar->addAction(addIPv6Action);
    toolbar->addAction(editAction);
    toolbar->addAction(deleteAction);
    toolbar->addAction(showInfoAction);
    toolbar->insertSeparator(showInfoAction);
    toolbar->addAction(configAction);
    toolbar->addAction(aboutAction);
    toolbar->addAction(quitAction);

    resetTitle();

    setWindowIcon(QIcon(":/appicon.svg"));

    searchField = new QLineEdit(this);
    searchField->setMaximumWidth(200);
    QLabel *searchLabel = new QLabel(tr("Search Host:"),this);
    QPushButton *searchButton = new QPushButton("Search",this);
    QPushButton *clearSearchButton = new QPushButton("Clear",this);

    connect(clearSearchButton,SIGNAL(clicked()),this,SLOT(searchFieldCleared()));
    connect(searchButton,SIGNAL(clicked()),this,SLOT(searchFieldChanged()));
    connect(searchField,SIGNAL(returnPressed()),this,SLOT(searchFieldChanged()));

    // Spacer, no function
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    toolbar->addWidget(empty);

    // ToolBar buttons for Map control
    QAction *xMinusAction = new QAction(tr("x-"),this);
    xMinusAction->setIcon(QIcon(":/xminus.svg"));
    ipv4toolbar->addAction(xMinusAction);
    connect(xMinusAction,SIGNAL(triggered()),map,SLOT(xWidthMinus()));

    QAction *xPlusAction = new QAction(tr("x+"),this);
    xPlusAction->setIcon(QIcon(":/xplus.svg"));
    ipv4toolbar->addAction(xPlusAction);
    connect(xPlusAction,SIGNAL(triggered()),map,SLOT(xWidthPlus()));

    QAction *yMinusAction = new QAction(tr("y-"),this);
    yMinusAction->setIcon(QIcon(":/yminus.svg"));
    ipv4toolbar->addAction(yMinusAction);
    connect(yMinusAction,SIGNAL(triggered()),map,SLOT(line_heightMinus()));

    QAction *yPlusAction = new QAction(tr("y+"),this);
    yPlusAction->setIcon(QIcon(":/yplus.svg"));
    ipv4toolbar->addAction(yPlusAction);
    connect(yPlusAction,SIGNAL(triggered()),map,SLOT(line_heightPlus()));

    QAction *autoResizeAction = new QAction(tr("<-auto->"),this);
    autoResizeAction->setIcon(QIcon(":/autoscale.svg"));
    ipv4toolbar->addAction(autoResizeAction);
    connect(autoResizeAction,SIGNAL(triggered()),map,SLOT(upscale()));

    connect(xMinusAction,SIGNAL(triggered()),this,SLOT(killAutoResize()));
    connect(yMinusAction,SIGNAL(triggered()),this,SLOT(killAutoResize()));
    connect(xPlusAction,SIGNAL(triggered()),this,SLOT(killAutoResize()));
    connect(yPlusAction,SIGNAL(triggered()),this,SLOT(killAutoResize()));

    toolbar->addSeparator();

    ipv6toolbar->addWidget(new QLabel("IPv6 tree generation margin: /",this));
    ipv6toolbar->addWidget(ipv6Scale);

    ipv4toolbar->addSeparator();
    ipv4toolbar->addWidget(autoResizeOption);
    ipv4toolbar->addSeparator();
    toolbar->addWidget(searchLabel);
    toolbar->addWidget(searchField);
    toolbar->addWidget(searchButton);
    toolbar->addWidget(clearSearchButton);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    parseRecentDocuments();

}

MainWindow::~MainWindow()
{
  QSettings settings;

  if (changedButNotSaved) {

      if (QMessageBox::question(this,"SubnetMap not saved!","this map has been altered, but not saved. Do you want to save it now before exit?",QMessageBox::Ok,QMessageBox::Cancel)==QMessageBox::Ok)
      {
          saveFile();
      }
  }

  infoDock->setSubnet(0);

  settings.setValue("mainwindow/width",window()->width());
  settings.setValue("mainwindow/height",window()->height());

  modelBackend->clearData();
  delete modelBackend;

}

QSize MainWindow::sizeHint() const
{
    QSettings settings;
    QSize size;
    size.setWidth(settings.value("mainwindow/width",800).toUInt());
    size.setHeight(settings.value("mainwindow/height",600).toUInt());
    return size;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSettings settings;

    // If desired, autoscale the map following a window resize.
    if (settings.value("mainwindow/autoresize",0)==1) map->upscale();

    QWidget::resizeEvent(event);
}

void MainWindow::setupModel()
{
    QSettings settings;

    // initialize the new model backend
    modelBackend = new SM_ModelBackend(this);

    // check if there was a filename specified at the command line. If yes, load it.
    // otherwise check for autoload_map feature and use this.
    // this only works because of the &&, which skips the second part of the check if the first one failed already... Otherwise
    // this would force a SIGABRT because of an Array index failure.
    if (((qApp->arguments().count())>1)&&(!(qApp->arguments().at(1).isEmpty())))
    {
        openFile(qApp->arguments().at(1));
    }
    else if (settings.value("mainwindow/autoload_map",0)!=0) openFile(settings.value("mainwindow/autoload_map_path","").toString());

}

void MainWindow::setupViews()
{
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    tabArea=new QTabWidget(this);

    table = new QTableWidget;
    QScrollArea *scroller = new QScrollArea;
    map = new SM_SubnetWidget(scroller);

    infoDock = new SM_InfoDockWidget(0,this);
    infoDock->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, infoDock);
    infoDock->setSubnet(0);

    QSettings settings;

    if (settings.value("mainwindow/infoPaneState",1)==1) infoDock->setVisible(true);
    else infoDock->setVisible(false);

    scroller->setWidget(map);
    scroller->setWidgetResizable(false);
    scroller->setStyleSheet(tr("QScrollArea { background-color: white }"));
    scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QVBoxLayout *ipv4layout=new QVBoxLayout;
    ipv4layout->addWidget(ipv4toolbar);
    ipv4layout->addWidget(scroller);
    QWidget *momipv4widget= new QWidget;
    momipv4widget->setLayout(ipv4layout);

    tabArea->addTab(momipv4widget,tr("IPv4 Map"));

    QVBoxLayout *ipv6layout=new QVBoxLayout;
    map6 = new QTreeWidget();
    ipv6layout->addWidget(ipv6toolbar);
    ipv6layout->addWidget(map6);
    QWidget *momipv6widget= new QWidget;
    momipv6widget->setLayout(ipv6layout);

    tabArea->addTab(momipv6widget,tr("IPv6 Map"));

    splitter->addWidget(tabArea);
    splitter->addWidget(table);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    connect(autoResizeOption,SIGNAL(clicked()),this,SLOT(autoResizeClicked()));

    connect(modelBackend,SIGNAL(dataChanged()),infoDock,SLOT(updateSubnet()));
    connect(modelBackend,SIGNAL(data4Changed()),infoDock,SLOT(updateSubnet()));
    connect(modelBackend,SIGNAL(data6Changed()),infoDock,SLOT(updateSubnet()));
    connect(modelBackend,SIGNAL(data4Changed()),map,SLOT(dataHasChanged()));
    connect(modelBackend,SIGNAL(dataChanged()),map,SLOT(dataHasChanged()));
    connect(table,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),this,SLOT(subnetTableItemChanged(QTableWidgetItem*,QTableWidgetItem*)));

    // bind our model to the views

    table->setSortingEnabled(false);
    table->setSelectionBehavior(QTableWidget::SelectRows);
    table->setSelectionMode(QTableWidget::SingleSelection);

    map->setModelBackend(modelBackend);

    map6->setSortingEnabled(false);
    map6->setSelectionBehavior(QAbstractItemView::SelectRows);
    map6->setSelectionMode(QAbstractItemView::SingleSelection);
    map6->setUniformRowHeights(true);
    map6->setColumnCount(5);
    map6->setExpandsOnDoubleClick(false);
    map6->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList headers;
    headers.append("");
    headers.append("");
    headers.append("Identifier");
    headers.append("IP Address");
    headers.append("Description");
    map6->setHeaderLabels(headers);

    connect(map6,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(map6ItemClicked(QTreeWidgetItem*,int)));

    QHeaderView *headerView = table->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    QHeaderView *headerSideView = table->verticalHeader();
    headerSideView->hide();

    setCentralWidget(splitter);

    modelDataHasChanged();
}

void MainWindow::addRecentDocument(QString filename)
{
    QSettings settings;

    int max_docs=settings.value("mainwindow/number_of_recent_docs",5).toInt();

    recentDocs.append(filename);

    while (recentDocs.count()>max_docs)
        recentDocs.removeLast();

    for (int i=0;i<recentDocs.count();i++)
    {
        settings.setValue("mainwindow/recent_doc_"+QString::number(i),recentDocs.at(i));
    }

    generateRecentDocsMenu();
}

void MainWindow::parseRecentDocuments()
{
    QSettings settings;

    int max_docs=settings.value("mainwindow/number_of_recent_docs",5).toInt();

    recentDocs.clear();

    for (int i=0;i<max_docs;i++)
    {
        recentDocs.append(settings.value("mainwindow/recent_doc_"+QString::number(i),"").toString());
    }

    generateRecentDocsMenu();
}

void MainWindow::generateRecentDocsMenu()
{
    recentDocsMenu->clear();

    for (int i=0;i<recentDocs.count();i++) {
        if (!recentDocs.at(i).isEmpty()) {
            QAction *momAction = recentDocsMenu->addAction(QIcon(),recentDocs.at(i).split('/').last(),this,SLOT(recentDocsMenuTrigger()));
            momAction->setData(recentDocs.at(i));
        };
    }

}

void MainWindow::showAboutDialog()
{
    SM_AboutDialog aboutDialog(this);

    aboutDialog.setModal(true);
    aboutDialog.exec();
}

void MainWindow::openFile(const QString &path)
{
    if (changedButNotSaved) {

        if (QMessageBox::question(this,"SubnetMap not saved!","this map has been altered, but not saved. Do you want to save it now before opening another map?",QMessageBox::Ok,QMessageBox::Cancel)==QMessageBox::Ok)
        {
            saveFile();
        }
    }

    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                                "", "*.smap");
    else
        fileName = path;

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {

            QDomDocument doc("SubnetMap");
            bool result=false;

            QString errMsg;
            int line,col;

            result=doc.setContent(&file,&errMsg,&line,&col);

            if (!result) {

                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText("Failed to parse the XML data in this file: "+errMsg+" in line "+QString::number(line)+" at column "+QString::number(col)+"!");
                msgBox.exec();

                file.close();
                return;
            }


            result=false;
            result=modelBackend->loadFromDomDoc(doc);

            file.close();

            if (result) {
                statusBar()->showMessage(tr("Loaded %1").arg(fileName), 5000);
                changedButNotSaved=false;
                resetTitle();
                addRecentDocument(fileName);
            } else statusBar()->showMessage(tr("Failed to parse SubnetMap %1").arg(fileName),5000);
        } else {

            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Failed to open file: "+fileName);
            msgBox.exec();
        };
    }
}

void MainWindow::updateSubnetTable()
{
    // clear the current table
    table->clear();

    // set the dimensions of the table
    table->setRowCount(modelBackend->count());
    table->setColumnCount(5);

    // prepare headers
    QStringList headers;
    headers.append("");
    headers.append("Identifier");
    headers.append("IP Address");
    headers.append("Netmask");
    headers.append("Description");
    table->setHorizontalHeaderLabels(headers);

    // item for the icon display in the header
    QTableWidgetItem *tableHeaderItem1 = new QTableWidgetItem(tr(""));
    tableHeaderItem1->setIcon(QIcon(QPixmap(":info.svg")));
    tableHeaderItem1->setTextAlignment(Qt::AlignHCenter);
    table->setHorizontalHeaderItem(0,tableHeaderItem1);

    // store the selected subnet
    int selectedItem=modelBackend->getSelectedIndex();

    // iterate all subnets
    for (int i=0;i<modelBackend->count();i++) {

        // create the item and populate it. pretty straightforward.
        QTableWidgetItem *itemColColor=new QTableWidgetItem();
        itemColColor->setBackgroundColor(modelBackend->getSubnet(i)->getColor());
        if (modelBackend->getSubnet(i)->getNotes()!="") itemColColor->setIcon(QIcon(QPixmap(":info.svg")));
        itemColColor->setTextAlignment(Qt::AlignHCenter);
        QTableWidgetItem *itemColIdentifier=new QTableWidgetItem(modelBackend->getSubnet(i)->getIdentifier());
        QTableWidgetItem *itemColIPAddress=new QTableWidgetItem(modelBackend->getSubnet(i)->toString());
        QTableWidgetItem *itemColNetmask;
        if (modelBackend->getSubnet(i)->isV4()) itemColNetmask=new QTableWidgetItem(modelBackend->getSubnet(i)->getStrNM());
        else itemColNetmask=new QTableWidgetItem(((Subnet_v6*)modelBackend->getSubnet(i))->getReducedStrNM());
        QTableWidgetItem *itemColDescription=new QTableWidgetItem(modelBackend->getSubnet(i)->getDescription());
        table->setItem(i,0,itemColColor);
        table->setItem(i,1,itemColIdentifier);
        table->setItem(i,2,itemColIPAddress);
        table->setItem(i,3,itemColNetmask);
        table->setItem(i,4,itemColDescription);
        // if subnet is selected, select Item, too.
        if (i==selectedItem) {
            itemColColor->setSelected(true);
            itemColIdentifier->setSelected(true);
            itemColIPAddress->setSelected(true);
            itemColNetmask->setSelected(true);
            itemColDescription->setSelected(true);
        };
    };

    // scroll there to show it off
    table->scrollToItem(table->item(selectedItem,0),QAbstractItemView::PositionAtCenter);


    //qDebug("MainWindow::updateSubnetTable(): finished.");

};

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save file as"), "", "*.smap");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        // QTextStream stream(&file);
        QXmlStreamWriter stream(&file);

        if (file.open(QFile::WriteOnly | QFile::Text)) {
            modelBackend->saveToXmlStream(stream);
        }

        file.close();
        statusBar()->showMessage(tr("Saved %1").arg(fileName), 5000);
        resetTitle();
        changedButNotSaved=false;
    }
}

void MainWindow::printFile()
{
    // Quick and dirty hack to get it working. No real features here, but the result is ok...

    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted) return;

    QPainter painter;
    painter.begin(&printer);
    double xscale = printer.pageRect().width()/double(map->width());
    double yscale = printer.pageRect().height()/double(map->height());
    double scale = qMin(xscale, yscale);
    painter.scale(scale, scale);
    map->paintJob(&painter,printer.pageRect());
    painter.end();
};

void MainWindow::addIPv4Subnet()
{
    sm_IPv4EditDialog editor(this);

    editor.setDescription("n/a");
    editor.setIdentifier("n/a");

    editor.setModal(true);
    if (editor.exec()==QDialog::Accepted) {

        QString momdesc = editor.getDescription();
        QString momid = editor.getIdentifier();
        QColor momcolor = editor.getColor();

        Subnet *newSubnet = new Subnet_v4(editor.getIP(),editor.getNM());
        newSubnet->setDescription(momdesc);
        newSubnet->setIdentifier(momid);
        newSubnet->setColor(momcolor);

        bool isNotOverlappingWithAnything=true;

        for (int i=0;i<modelBackend->count();i++) {
            if (modelBackend->getSubnet(i)->isV4())
                if (((Subnet_v4*)(modelBackend->getSubnet(i)))->overlapsWith(*((Subnet_v4*)newSubnet)))
                    isNotOverlappingWithAnything=false;
        }

        if (isNotOverlappingWithAnything) {
            modelBackend->addSubnet(newSubnet);
            mapWasAltered();
        } else {
            QMessageBox msgBox;
            msgBox.setText("The Subnet you specified overlaps with an existing subnet.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setDetailedText("The overlapping of subnets in a single map is not allowed in SubnetMapper. The author of this program cannot think of a situation in the real world where this could be an advisable situation. If you find yourself in a mess like this, please think it over, stuff like that always catches up with you at a later point in time, when you are actually least expecting it.");
            msgBox.exec();
        }

    }


}

void MainWindow::addIPv6Subnet()
{
    SM_IPv6EditDialog editor(this);

    editor.setModal(true);

    editor.setDescription("n/a");
    editor.setIdentifier("n/a");

    if (editor.exec()==QDialog::Accepted) {

        QString momdesc = editor.getDescription();
        QString momid = editor.getIdentifier();

        Subnet *newSubnet = new Subnet_v6(editor.getIP(),editor.getNM());
        newSubnet->setDescription(momdesc);
        newSubnet->setIdentifier(momid);

        bool isNotOverlappingWithAnything=true;

        for (int i=0;i<modelBackend->count();i++) {
            if (modelBackend->getSubnet(i)->isV6())
                if (((Subnet_v6*)(modelBackend->getSubnet(i)))->overlapsWith(*((Subnet_v6*)newSubnet)))
                    isNotOverlappingWithAnything=false;
        }

        if (isNotOverlappingWithAnything) {
            modelBackend->addSubnet(newSubnet);
            mapWasAltered();
        } else {
            QMessageBox msgBox;
            msgBox.setText("The Subnet you specified overlaps with an existing subnet.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setDetailedText("The overlapping of subnets in a single map is not allowed in SubnetMapper. The author of this program cannot think of a situation in the real world where this could be an advisable situation. If you find yourself in a mess like this, please think it over, stuff like that always catches up with you at a later point in time, when you are actually least expecting it.");
            msgBox.exec();
        }

    }

}

void MainWindow::searchFieldChanged()
{
    if (map->searchHosts(searchField->text()))
    {
        if (searchField->text().isEmpty()) searchField->setStyleSheet("QLineEdit { background-color: white; }");
        else  searchField->setStyleSheet("QLineEdit { background-color: lightgreen; }");

    } else {
        searchField->setStyleSheet("QLineEdit { background-color: #FF8080; }");
    }
}

void MainWindow::searchFieldCleared()
{
    searchField->clear();
    searchFieldChanged();
}

void MainWindow::deleteCurrentSubnet()
{
    infoDock->setSubnet(0);

    int selection=modelBackend->getSelectedIndex();
    if (selection<=modelBackend->count()) {
        modelBackend->removeSubnet(selection);
        mapWasAltered();
    };
}

void MainWindow::editCurrentSubnet()
{
    int selection=modelBackend->getSelectedIndex();

    if (modelBackend->getSubnet(selection)->isV4())
    {
        if (map) map->editCurrentSubnet();
        mapWasAltered();
    } else {

        Subnet_v6* subnet6=modelBackend->getSubnet6(modelBackend->indexAllto6(selection));

        SM_IPv6EditDialog editor(this);

        editor.setModal(true);

        editor.setDescription(subnet6->getDescription());
        editor.setIdentifier(subnet6->getIdentifier());
        editor.setNM(subnet6->getNormalizedStrNM());
        editor.setIP(subnet6->toNormalizedString());
        editor.setColor(subnet6->getColor());
        editor.updateFields();

        if (editor.exec()==QDialog::Accepted) {

            QString momdesc = editor.getDescription();
            QString momid = editor.getIdentifier();

            Subnet *newSubnet = new Subnet_v6(editor.getIP(),editor.getNM());
            newSubnet->setDescription(momdesc);
            newSubnet->setIdentifier(momid);

            bool isNotOverlappingWithAnything=true;

            for (int i=0;i<modelBackend->count();i++) {
                if (modelBackend->getSubnet(i)->isV6())
                    if (((Subnet_v6*)(modelBackend->getSubnet(i)))->overlapsWith(*((Subnet_v6*)newSubnet)))
                        isNotOverlappingWithAnything=false;
            }

            if (isNotOverlappingWithAnything) {
                subnet6->setDescription(momdesc);
                subnet6->setIdentifier(momid);
                subnet6->setColor(editor.getColor());
                QString momIP=editor.getIP();
                QString momNM=editor.getNM();
                subnet6->setIP(momIP);
                subnet6->setNM(momNM);
                mapWasAltered();
            } else {
                QMessageBox msgBox;
                msgBox.setText("The Subnet you specified overlaps with an existing subnet.");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setDetailedText("The overlapping of subnets in a single map is not allowed in SubnetMapper. The author of this program cannot think of a situation in the real world where this could be an advisable situation. If you find yourself in a mess like this, please think it over, stuff like that always catches up with you at a later point in time, when you are actually least expecting it.");
                msgBox.exec();
            }

        }


    }
}

void MainWindow::showInfoPane()
{
    QSettings settings;
    infoDock->setVisible(!(infoDock->isVisible()));
    if (infoDock->isVisible()) settings.setValue("mainwindow/infoPaneState",1);
    else settings.setValue("mainwindow/infoPaneState",0);
    qDebug("infoPaneState: %u",settings.value("mainwindow/infoPaneState",1).toUInt());
}

void MainWindow::showConfigDialog()
{
    SM_ConfigDialog configDialog(this);

    if (configDialog.exec()==QDialog::Accepted) {
        parseRecentDocuments();
        generateRecentDocsMenu();
    }
}

void MainWindow::modelDataHasChanged()
{
    //qDebug("MainWindow::modelDataHasChanged() called...");
    updateSubnetTable();
    updateIPv6Map();
}

void MainWindow::selectionChanged()
{
    //qDebug("MainWindow::selectionChanged() called...");
    int selection=modelBackend->getSelectedIndex();

    if (modelBackend->getSubnet(selection)) {
        if (modelBackend->getSubnet(selection)->isV4()) tabArea->setCurrentIndex(0);
        else tabArea->setCurrentIndex(1);
    };

    if (selection<=modelBackend->count()) {
        infoDock->setSubnet(modelBackend->getSubnet(selection));
    } else infoDock->setSubnet(0);
}

void MainWindow::autoResizeClicked()
{
    QSettings settings;

    if (autoResizeOption->isChecked()) {
        settings.setValue("mainwindow/autoresize",1);
        map->upscale();
    } else
        settings.setValue("mainwindow/autoresize",0);
}

void MainWindow::subnetTableItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous)
{
    // if nothing changes, just quit, because we would fall in an endless loop otherwise
    if (current==previous) return;

    // if no selection available, do nothing. if else, select the item.
    if (current!=NULL)
    {
        qDebug("MainWindow::subnetTableItemChanged(): new selection in subnetTable detected, row %d...",current->row());
        modelBackend->selectIndex(current->row());
        selectionChanged();
    };
}

void MainWindow::subnetTableSelectRow(int index)
{
    if ((index>=0)&(index<modelBackend->count())){
        modelBackend->selectIndex(index);
    }
}

void MainWindow::killAutoResize()
{
    QSettings settings;
    settings.setValue("mainwindow/autoresize",0);
    autoResizeOption->setChecked(false);
}

void MainWindow::recentDocsMenuTrigger()
{
    openFile(((QAction*)sender())->data().toString());
}

void MainWindow::mapWasAltered()
{
    setWindowTitle(this->windowTitle()+" (*)");
    changedButNotSaved=true;
}

void MainWindow::resetTitle()
{
    setWindowTitle(tr("SubnetMapper V")+qApp->applicationVersion());
}

void MainWindow::updateIPv6Map()
{
    // clear the map for repopulation
    map6->clear();

    // store the current scale setting to presets
    QSettings settings;
    settings.setValue("mainwindow/ipv6scale",ipv6Scale->value());

    // recurse through out data to get the whole subtree
    QTreeWidgetItem* resultingIPv6Tree = map6RecursivePopulator(QString());

    // and add it to the map
    map6->addTopLevelItem(resultingIPv6Tree);

    // tidy up display.
    map6->expandAll();
    map6->resizeColumnToContents(0);
    map6->resizeColumnToContents(1);
    map6->resizeColumnToContents(2);
    map6->resizeColumnToContents(3);
    map6->resizeColumnToContents(4);

    // take care of selection.
    if ((modelBackend->getSelectedIndex()<=modelBackend->count())&&(modelBackend->getSubnet(modelBackend->getSelectedIndex())->isV6())) {
        Subnet_v6* subnet=modelBackend->getSubnet6(modelBackend->indexAllto6(modelBackend->getSelectedIndex()));
        QList<QTreeWidgetItem*> result=map6->findItems(subnet->toString(),Qt::MatchFixedString|Qt::MatchCaseSensitive|Qt::MatchRecursive,3);
        if (result.count()==1) map6->setCurrentItem(result.at(0));
    }

}

// call with an empty string and receive the top-level object for map6 (recursion inside)
QTreeWidgetItem* MainWindow::map6RecursivePopulator(QString prefix)
{
    // Storage for returned Items
    QList<QTreeWidgetItem*> itemList;

    if (prefix.count()<=0) {
        // empty input. we have to start the recursion.
        QStringList prefixList;

        // lets iterate through all nets and add them to our String List. But we only add the first two characters,
        // as we only want to start the recursion here. the colons are not necessary anymore, too.
        for (int i=0;i<modelBackend->count6();i++) {
            prefixList.append(modelBackend->getSubnet6(i)->toNormalizedString().remove(':').left(2));
        }

        // sort and dupe kill all the prefixes.
        prefixList.sort();
        prefixList.removeDuplicates();

        // now recurse over our prefix list!
        for (int i=0;i<prefixList.count();i++) {
            itemList.append(map6RecursivePopulator(prefixList.at(i)));
        }
    } else {
        // if the prefix is longer than zero, we are already in a running recursion,
        // so lets search for all nets in our datamodel which start with our prefix,
        // and recurse deeper. but only as far as we configured via out ipv6Scale Spinbox.
        if (prefix.length()<(ipv6Scale->value()/4)) {

            QStringList prefixList;

            // principle stays the same, iterate over all nets, check if they
            // are part of the prefix, if yes, append them to the list.
            for (int i=0;i<modelBackend->count6();i++) {
                QString subnetaddr=modelBackend->getSubnet6(i)->toNormalizedString().remove(':');
                if (subnetaddr.startsWith(prefix)) prefixList.append(subnetaddr.left(prefix.length()+2));
            }

            // sort and dupekill the prefixes.
            prefixList.sort();
            prefixList.removeDuplicates();

            // recurse deeper and deeper...
            for (int i=0;i<prefixList.count();i++) {
                itemList.append(map6RecursivePopulator(prefixList.at(i)));
            }

        } else {

            // we reached the end/leaf of our recursion. So iterate all nets again. Check if they are part
            // of our prefix. If yes, create a node and append them to the node list (itemList) for later
            // tree integration.
            for (int i=0;i<modelBackend->count6();i++) {
                QString subnetaddr=modelBackend->getSubnet6(i)->toNormalizedString().remove(':');
                if (subnetaddr.startsWith(prefix)) {
                    QStringList mom;
                    mom.append(QString());
                    mom.append(QString("  "));
                    mom.append(modelBackend->getSubnet6(i)->getIdentifier());
                    mom.append(modelBackend->getSubnet6(i)->toString());
                    mom.append(modelBackend->getSubnet6(i)->getDescription());
                    QTreeWidgetItem* terminalNode=new QTreeWidgetItem(mom);
                    if ((modelBackend->getSubnet6(i)->getNotes()!="")&(modelBackend->getSubnet6(i)->getNotes()!="n/a")) terminalNode->setIcon(1,QIcon(QPixmap(":info.svg")));
                    terminalNode->setBackgroundColor(1,modelBackend->getSubnet6(i)->getColor());
                    itemList.append(terminalNode);
                };
            }
        }
    }

    // This is it. We have iterated enough, now we create the tree data structure. All begins
    // at the leafs. there we create the real Items with subnets in them. These Items are
    // integrated into subtrees via their common subnets, as long until we reach the top level
    // node.

    // We are not interested in all possible dead ends of our recursion, so lets just throw them out
    itemList.removeAll(NULL);

    // Only return valid objects or NULL.
    if (itemList.count()==0) return NULL;

    // if we only would create a node which only carries a single child, return the child to
    // save unecessary redundant structure, we would not gain any more information from it anyway.
    if ((itemList.count()==1)&(itemList.at(0)->childCount()==1)) return itemList.at(0);

    // lets fix up the display of our shared subnet nodes to only display the relevant address data.
    prefix=prefix.left((ipv6Scale->value()/4));
    int prefixlength=prefix.length();
    QString newPrefix="";
    // reinsert colons
    for (int i=0;i<prefixlength/4;i++) newPrefix+=prefix.mid(i*4,4)+":";
    // cut the last colon
    prefix=newPrefix.left(newPrefix.length()-1);
    // add CIDR info
    prefix.sprintf("%s::/%u",qPrintable(prefix.toUtf8()),prefixlength*4);
    // reduce IP
    prefix=Subnet_v6::reduceIP(Subnet_v6::normalizeIP(prefix));

    // prepare the TreeItem for our structure.
    QStringList mom;
    mom.append(QString(prefix));
    QTreeWidgetItem* newNode=new QTreeWidgetItem(mom);
    newNode->setForeground(0,QBrush(Qt::darkGray));
    newNode->setFlags(Qt::ItemIsEnabled);

    // add the returned nodes to our structure item
    newNode->addChildren(itemList);

    // return our subtree.
    return newNode;
}

void MainWindow::map6ItemClicked(QTreeWidgetItem * item, int)
{
    if (item!=NULL) {
        QString selectedItemText=item->text(3);
        bool foundSelectedSubnet=false;
        for (int i=0;i<modelBackend->count6();i++) {
            if (modelBackend->getSubnet6(i)->toString()==selectedItemText) {
                foundSelectedSubnet=true;
                modelBackend->selectIndex(modelBackend->index6toAll(i));
            }
        };
        if (foundSelectedSubnet) selectionChanged();
    }
}

