#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QItemSelectionModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile(const QString &path = QString());
    void saveFile();

private:
    void setupModel();
    void setupViews();

    QAbstractTableModel *model;
    QAbstractItemView *map;
    QItemSelectionModel *selectionModel;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
