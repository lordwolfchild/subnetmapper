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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QSettings settings;

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

    // Print Map
    QAction *printAction = fileMenu->addAction(tr("&Print..."));
    printAction->setShortcut(QKeySequence(tr("Ctrl+P")));
    printAction->setIcon(QIcon(":/printer.svg"));

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
    connect(printAction,SIGNAL(triggered()),this,SLOT(printFile()));
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),map,SLOT(dataHasChanged()));

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    this->menuBar()->addMenu(helpMenu);
    statusBar();

    QToolBar *toolbar = new QToolBar("Main Toolbar",this);
    this->addToolBar(Qt::TopToolBarArea,toolbar);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->addAction(openAction);
    toolbar->addAction(saveAction);
    toolbar->addAction(printAction);
    toolbar->addAction(addIPv4Action);
    toolbar->insertSeparator(addIPv4Action);
    toolbar->addAction(addIPv6Action);
    toolbar->addAction(aboutAction);
    toolbar->addAction(quitAction);
    toolbar->insertSeparator(aboutAction);

    setWindowTitle(tr("SubnetMapper V")+qApp->applicationVersion());

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

    toolbar->addSeparator();
    toolbar->addWidget(empty);
    toolbar->addWidget(searchLabel);
    toolbar->addWidget(searchField);
    toolbar->addWidget(searchButton);
    toolbar->addWidget(clearSearchButton);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

}

MainWindow::~MainWindow()
{
  QSettings settings;

  settings.setValue("mainwindow/width",window()->width());
  settings.setValue("mainwindow/height",window()->height());
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
    //map->upscale();
    QWidget::resizeEvent(event);
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
    table = new QTableView;
    QScrollArea *scroller = new QScrollArea;
    map = new SM_SubnetWidget(this);

    scroller->setWidget(map);
    scroller->setWidgetResizable(false);
    scroller->setStyleSheet(tr("QScrollArea { background-color: white }"));
    scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    splitter->addWidget(scroller);
    splitter->addWidget(table);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    QPushButton *buttonXplus  = new QPushButton("+",this);
    QPushButton *buttonXminus = new QPushButton("-",this);
    QPushButton *buttonYplus  = new QPushButton("+",this);
    QPushButton *buttonYminus = new QPushButton("-",this);
    QPushButton *buttonUpscale = new QPushButton("o",this);
    buttonXplus->setMaximumWidth(30);
    buttonXminus->setMaximumWidth(30);
    buttonYplus->setMaximumWidth(30);
    buttonYminus->setMaximumWidth(30);
    buttonUpscale->setMaximumWidth(15);

    scroller->addScrollBarWidget(buttonXplus,Qt::AlignRight);
    scroller->addScrollBarWidget(buttonXminus,Qt::AlignRight);
    scroller->addScrollBarWidget(buttonUpscale,Qt::AlignBottom);
    scroller->addScrollBarWidget(buttonYplus,Qt::AlignBottom);
    scroller->addScrollBarWidget(buttonYminus,Qt::AlignBottom);


    (scroller->horizontalScrollBar())->setMinimumWidth(width());
    (scroller->verticalScrollBar())->setMinimumHeight(height());
    (scroller->verticalScrollBar())->setBaseSize(200,20);
    (scroller->horizontalScrollBar())->setBaseSize(20,200);

    connect(buttonXplus,SIGNAL(clicked()),map,SLOT(xWidthPlus()));
    connect(buttonXminus,SIGNAL(clicked()),map,SLOT(xWidthMinus()));
    connect(buttonYplus,SIGNAL(clicked()),map,SLOT(line_heightPlus()));
    connect(buttonYminus,SIGNAL(clicked()),map,SLOT(line_heightMinus()));
    connect(buttonUpscale,SIGNAL(clicked()),map,SLOT(upscale()));

    // bind our model to the views. Be advised that SM_subnetwidget is  not a Model/View aware class, but
    // only emulates this behaviour in necessary boundaries to retrieve its data from the model and the selection.
    // Do not try fancy stuff with this class (without knowing what you do, of course)!
    table->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    table->setSortingEnabled(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    map->setModel((SM_DataModel*)model);
    map->setSelectionModel(selectionModel);

    QHeaderView *headerView = table->horizontalHeader();
    headerView->setResizeMode(QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);

    QHeaderView *headerSideView = table->verticalHeader();
    headerSideView->hide();

    setCentralWidget(splitter);

    map->upscale();

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

            if (result) {
                statusBar()->showMessage(tr("Loaded %1").arg(fileName), 5000);
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
    }
}

void MainWindow::printFile()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted) return;

    QPainter painter;
    painter.begin(&printer);
    double xscale = printer.pageRect().width()/double(map->width());
    double yscale = printer.pageRect().height()/double(map->height());
    double scale = qMin(xscale, yscale);
    //painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                      //printer.paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    //painter.translate(-width()/2, -height()/2);

    map->render(&painter);
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

        if (isNotOverlappingWithAnything) {
            map->repaint();
        };

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
