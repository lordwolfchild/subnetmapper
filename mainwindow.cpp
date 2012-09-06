#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sm_datamodel.h"
#include <QSplitter>
#include <QTableView>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setIcon(QIcon(":/open.svg"));
    QAction *saveAction = fileMenu->addAction(tr("&Save As..."));
    saveAction->setShortcuts(QKeySequence::SaveAs);
    saveAction->setIcon(QIcon(":/save.svg"));
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setIcon(QIcon(":/quit.svg"));

    setupModel();
    setupViews();

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    menuBar()->addMenu(fileMenu);
    statusBar();

    QToolBar *toolbar = new QToolBar("Main Toolbar",this);
    this->addToolBar(Qt::LeftToolBarArea,toolbar);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->addAction(openAction);
    toolbar->addAction(saveAction);
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
    QTableView *table = new QTableView;
    //map = new SM_mapView;
    splitter->addWidget(table);
    //splitter->addWidget(pieChart);
    splitter->setStretchFactor(0, 1);
    //splitter->setStretchFactor(1, 1);

    table->setModel(model);
    //pieChart->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
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
/*    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                                "", "*.cht");
    else
        fileName = path;

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
/*    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save file as"), "", "*.cht");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        QTextStream stream(&file);

        if (file.open(QFile::WriteOnly | QFile::Text)) {
            for (int row = 0; row < model->rowCount(QModelIndex()); ++row) {

                QStringList pieces;

                pieces.append(model->data(model->index(row, 0, QModelIndex()),
                                          Qt::DisplayRole).toString());
                pieces.append(model->data(model->index(row, 1, QModelIndex()),
                                          Qt::DisplayRole).toString());
                pieces.append(model->data(model->index(row, 0, QModelIndex()),
                                          Qt::DecorationRole).toString());

                stream << pieces.join(",") << "\n";
            }
        }

        file.close();
        statusBar()->showMessage(tr("Saved %1").arg(fileName), 2000);
    }*/
}
