#include "mainwindow.h"
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
#include <QScrollArea>
#include <QMessageBox>
#include "sm_aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QMenu *editMenu = new QMenu(tr("&Edit"), this);
    QMenu *helpMenu = new QMenu(tr("&Help"), this);

    // Open File
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setIcon(QIcon(":/open.svg"));

    // Save File
    QAction *saveAction = fileMenu->addAction(tr("&Save As..."));
    saveAction->setShortcuts(QKeySequence::SaveAs);
    saveAction->setIcon(QIcon(":/save.svg"));

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

    // About Dialog
    QAction *aboutAction = helpMenu->addAction(tr("&About SubnetMapper"));
    aboutAction->setIcon(QIcon(":/appicon.svg"));


    setupModel();
    setupViews();

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(addIPv4Action, SIGNAL(triggered()), this, SLOT(addIPv4Subnet()));
    connect(addIPv6Action, SIGNAL(triggered()), this, SLOT(addIPv6Subnet()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),map,SLOT(dataHasChanged()));

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    this->menuBar()->addMenu(helpMenu);
    statusBar();

    QToolBar *toolbar = new QToolBar("Main Toolbar",this);
    this->addToolBar(Qt::LeftToolBarArea,toolbar);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->addAction(openAction);
    toolbar->addAction(saveAction);
    toolbar->addAction(addIPv4Action);
    toolbar->insertSeparator(addIPv4Action);
    toolbar->addAction(addIPv6Action);
    toolbar->addAction(aboutAction);
    toolbar->addAction(quitAction);
    toolbar->insertSeparator(aboutAction);

    setWindowTitle(tr("SubnetMapper V2.0.0"));

    resize(1000,600);

    setWindowIcon(QIcon(":/appicon.svg"));

}

MainWindow::~MainWindow()
{
}


void MainWindow::setupModel()
{
    model = new SM_DataModel(this);
    ((SM_DataModel*)model)->addDemos();
}

void MainWindow::setupViews()
{
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    QTableView *table = new QTableView;
    QScrollArea *scroller = new QScrollArea;
    map = new SM_SubnetWidget;

    scroller->setWidget(map);
    scroller->setWidgetResizable(false);
    scroller->setStyleSheet(tr("QScrollArea { background-color: white }"));

    splitter->addWidget(scroller);
    splitter->addWidget(table);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    // bind our model to the views. Be advised that SM_subnetwidget is  not a Model/View aware class, but
    // only emulates this behaviour in necessary boundaries to retrieve its data from the model and the selection.
    // Do not try fancy stuff with this class (without knowing what you do, of course)!
    table->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    table->setSortingEnabled(true);

    map->setModel((SM_DataModel*)model);
    map->setSelectionModel(selectionModel);

    QHeaderView *headerView = table->horizontalHeader();
    headerView->setResizeMode(QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    QHeaderView *headerSideView = table->verticalHeader();
    headerSideView->hide();

    setCentralWidget(splitter);

    SM_DataModel* momLink = (SM_DataModel*)model;

}

void MainWindow::showAboutDialog()
{
    SM_AboutDialog aboutDialog(this);

    aboutDialog.setModal(true);
    aboutDialog.exec();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                                "", "*.smap");
    else
        fileName = path;

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            /*
            QXmlStreamReader stream(&file);
            bool result;

            result=((SM_DataModel*)model)->loadFromXmlStream(stream);
            */

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

            file.close();

            if (result) statusBar()->showMessage(tr("Loaded %1").arg(fileName), 5000);
            else statusBar()->showMessage(tr("Failed to parse SubnetMap %1").arg(fileName),5000);
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
    }
}

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

        if (isNotOverlappingWithAnything) map->repaint();

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

        // TODO: check if subnet overlaps something we already have in this map

        ((SM_DataModel*)model)->addSubnet(newSubnet);

        map->repaint();

    }


}
