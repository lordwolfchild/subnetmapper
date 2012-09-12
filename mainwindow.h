#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QItemSelectionModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile(const QString &path = QString());
    void saveFile();

    void addIPv4Subnet();
    void addIPv6Subnet();


private:
    void setupModel();
    void setupViews();

    QAbstractTableModel *model;
    QAbstractItemView *map;
    QItemSelectionModel *selectionModel;

};

#endif // MAINWINDOW_H
