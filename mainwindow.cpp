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
#include "sm_datamodel.h"
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
#include "sm_subnet6widget.h"
#include "sm_model6proxy.h"
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
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),map,SLOT(dataHasChanged()));
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(modelDataHasChanged()));
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged()));
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

    ipv6Scale = new QSpinBox(this);
    ipv6Scale->setMaximumWidth(100);
    ipv6Scale->setMinimum(32);
    ipv6Scale->setMaximum(64);
    ipv6toolbar->addWidget(new QLabel("IPv6 Display: /",this));
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

  model->clearData();
  delete model;
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
    qDebug("Setting up Models...");
    QSettings settings;
    model = new SM_DataModel(this);

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
    qDebug("Setting up Views...");
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    tabArea=new QTabWidget(this);

    table = new QTableView;
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
    map6 = new SM_Subnet6Widget();
    ipv6layout->addWidget(ipv6toolbar);
    // TODO: Add real map here
    //ipv6layout->addWidget(new QWidget());
    ipv6layout->addWidget(map6);
    QWidget *momipv6widget= new QWidget;
    momipv6widget->setLayout(ipv6layout);

    tabArea->addTab(momipv6widget,tr("IPv6 Map"));

    splitter->addWidget(tabArea);
    splitter->addWidget(table);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

//    QPushButton *buttonXplus  = new QPushButton("+",this);
//    QPushButton *buttonXminus = new QPushButton("-",this);
//    QPushButton *buttonYplus  = new QPushButton("+",this);
//    QPushButton *buttonYminus = new QPushButton("-",this);
//    QPushButton *buttonUpscale = new QPushButton("o",this);
//    buttonXplus->setMaximumWidth(30);
//    buttonXminus->setMaximumWidth(30);
//    buttonYplus->setMaximumWidth(30);
//    buttonYminus->setMaximumWidth(30);
//    buttonUpscale->setMaximumWidth(15);

//    scroller->addScrollBarWidget(buttonXplus,Qt::AlignRight);
//    scroller->addScrollBarWidget(buttonXminus,Qt::AlignRight);
//    scroller->addScrollBarWidget(buttonUpscale,Qt::AlignBottom);
//    scroller->addScrollBarWidget(buttonYplus,Qt::AlignBottom);
//    scroller->addScrollBarWidget(buttonYminus,Qt::AlignBottom);

//    (scroller->horizontalScrollBar())->setMinimumWidth(width());
//    (scroller->verticalScrollBar())->setMinimumHeight(height());
//    (scroller->verticalScrollBar())->setBaseSize(200,20);
//    (scroller->horizontalScrollBar())->setBaseSize(20,200);

//    connect(buttonXplus,SIGNAL(clicked()),map,SLOT(xWidthPlus()));
//    connect(buttonXminus,SIGNAL(clicked()),map,SLOT(xWidthMinus()));
//    connect(buttonYplus,SIGNAL(clicked()),map,SLOT(line_heightPlus()));
//    connect(buttonYminus,SIGNAL(clicked()),map,SLOT(line_heightMinus()));
//    connect(buttonUpscale,SIGNAL(clicked()),map,SLOT(upscale()));

//    connect(buttonXplus,SIGNAL(clicked()),this,SLOT(killAutoResize()));
//    connect(buttonXminus,SIGNAL(clicked()),this,SLOT(killAutoResize()));
//    connect(buttonYplus,SIGNAL(clicked()),this,SLOT(killAutoResize()));
//    connect(buttonYminus,SIGNAL(clicked()),this,SLOT(killAutoResize()));

    connect(autoResizeOption,SIGNAL(clicked()),this,SLOT(autoResizeClicked()));

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),infoDock,SLOT(updateSubnet()));
    connect(modelBackend,SIGNAL(data4Changed()),map,SLOT(dataHasChanged()));
    connect(modelBackend,SIGNAL(dataChanged()),map,SLOT(dataHasChanged()));

    // bind our model to the views. Be advised that SM_subnetwidget is  not a Model/View aware class, but
    // only emulates this behaviour in necessary boundaries to retrieve its data from the model and the selection.
    // Do not try fancy stuff with this class (without knowing what you do, of course)!
    table->setModel(model);

    selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    table->setSortingEnabled(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    map->setModelBackend(modelBackend);

    map6->setModel(model);
    map6->setSelectionModel(selectionModel);
    map6->setSortingEnabled(false);
    map6->setSelectionBehavior(QAbstractItemView::SelectRows);
    map6->setSelectionMode(QAbstractItemView::SingleSelection);

    QHeaderView *headerView = table->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    QHeaderView *headerSideView = table->verticalHeader();
    headerSideView->hide();

    setCentralWidget(splitter);

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
            result=((SM_DataModel*)model)->loadFromDomDoc(doc);
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

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save file as"), "", "*.smap");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        // QTextStream stream(&file);
        QXmlStreamWriter stream(&file);

        if (file.open(QFile::WriteOnly | QFile::Text)) {

            ((SM_DataModel*)model)->saveToXmlStream(stream);
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

        for (int i=0;i<((SM_DataModel*)model)->rowCount();i++) {
            if (((SM_DataModel*)model)->getSubnet(i)->isV4())
                if (((Subnet_v4*)(((SM_DataModel*)model)->getSubnet(i)))->overlapsWith(*((Subnet_v4*)newSubnet)))
                    isNotOverlappingWithAnything=false;
        }

        if (isNotOverlappingWithAnything) ((SM_DataModel*)model)->addSubnet(newSubnet);
        else {
            QMessageBox msgBox;
            msgBox.setText("The Subnet you specified overlaps with an existing subnet.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setDetailedText("The overlapping of subnets in a single map is not allowed in SubnetMapper. The author of this program cannot think of a situation in the real world where this could be an advisable situation. If you find yourself in a mess like this, please think it over, stuff like that always catches up with you at a later point in time, when you are actually least expecting it.");
            msgBox.exec();
        }

        /* if (isNotOverlappingWithAnything) {  // not really necessary anymore, addSubnet triggers redraw
            mapWasAltered();
            map->repaint();
        };*/

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

        for (int i=0;i<((SM_DataModel*)model)->rowCount();i++) {
            if (((SM_DataModel*)model)->getSubnet(i)->isV6())
                if (((Subnet_v6*)(((SM_DataModel*)model)->getSubnet(i)))->overlapsWith(*((Subnet_v6*)newSubnet)))
                    isNotOverlappingWithAnything=false;
        }

        if (isNotOverlappingWithAnything) ((SM_DataModel*)model)->addSubnet(newSubnet);
        else {
            QMessageBox msgBox;
            msgBox.setText("The Subnet you specified overlaps with an existing subnet.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setDetailedText("The overlapping of subnets in a single map is not allowed in SubnetMapper. The author of this program cannot think of a situation in the real world where this could be an advisable situation. If you find yourself in a mess like this, please think it over, stuff like that always catches up with you at a later point in time, when you are actually least expecting it.");
            msgBox.exec();
        }

        mapWasAltered();
        map->repaint();

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
    if ((selectionModel->currentIndex().isValid())&(selectionModel->hasSelection())) {
        model->removeRows(selectionModel->currentIndex().row(),1);
        mapWasAltered();
    };
}

void MainWindow::editCurrentSubnet()
{
    if (map) map->editCurrentSubnet();
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
    std::cout << "TEST" << std::endl;
    table->reset();
}

void MainWindow::selectionChanged()
{
    if ((selectionModel->currentIndex().isValid())&(selectionModel->hasSelection())) {
        infoDock->setSubnet(model->getSubnet(selectionModel->currentIndex().row()));
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
