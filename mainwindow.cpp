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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QMenu *editMenu = new QMenu(tr("&Edit"), this);

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


    setupModel();
    setupViews();

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(addIPv4Action, SIGNAL(triggered()), this, SLOT(addIPv4Subnet()));
    connect(addIPv6Action, SIGNAL(triggered()), this, SLOT(addIPv6Subnet()));

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    statusBar();

    QToolBar *toolbar = new QToolBar("Main Toolbar",this);
    this->addToolBar(Qt::LeftToolBarArea,toolbar);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->addAction(openAction);
    toolbar->addAction(saveAction);
    toolbar->addAction(addIPv4Action);
    toolbar->insertSeparator(addIPv4Action);
    toolbar->addAction(addIPv6Action);
    toolbar->addAction(quitAction);
    toolbar->insertSeparator(quitAction);

    setWindowTitle(tr("SubnetMapper V0.0"));

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
    QListView *list = new QListView;

    //map = new SM_mapView;
    splitter->addWidget(list);
    splitter->addWidget(table);
    //splitter->addWidget(pieChart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    table->setModel(model);
    list->setModel(model);

    //pieChart->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    list->setSelectionModel(selectionModel);
    //pieChart->setSelectionModel(selectionModel);

    QHeaderView *headerView = table->horizontalHeader();
    headerView->setResizeMode(QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    QHeaderView *headerSideView = table->verticalHeader();
    headerSideView->hide();

    setCentralWidget(splitter);
}

void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                                "", "*.cht");
    else
        fileName = path;
/*
    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QString line;

            model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {

                    model->insertRows(row, 1, QModelIndex());

                    QStringList pieces = line.split(",", QString::SkipEmptyParts);
                    model->setData(model->index(row, 0, QModelIndex()),
                                   pieces.value(0));
                    model->setData(model->index(row, 1, QModelIndex()),
                                   pieces.value(1));
                    model->setData(model->index(row, 0, QModelIndex()),
                                   QColor(pieces.value(2)), Qt::DecorationRole);
                    row++;
                }
            } while (!line.isEmpty());

            file.close();
            statusBar()->showMessage(tr("Loaded %1").arg(fileName), 2000);
        }
    } */
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


        }

        file.close();
        statusBar()->showMessage(tr("Saved %1").arg(fileName), 2000);
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

        Subnet *newSubnet = new Subnet_v4(editor.getIP(),editor.getNM());
        newSubnet->setDescription(momdesc);
        newSubnet->setIdentifier(momid);
        ((SM_DataModel*)model)->addSubnet(newSubnet);

    }

}

void MainWindow::addIPv6Subnet()
{
    SM_IPv6EditDialog editor(this);

    editor.setModal(true);
    editor.exec();

}
