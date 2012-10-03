#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include "sm_subnetwidget.h"
#include <QItemSelectionModel>
#include "sm_datamodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile(const QString &path = QString());
    void saveFile();

    void showAboutDialog();

    void addIPv4Subnet();
    void addIPv6Subnet();

private:
    void setupModel();
    void setupViews();

    SM_DataModel *model;
    SM_SubnetWidget *map;
    QItemSelectionModel *selectionModel;

};

#endif // MAINWINDOW_H
