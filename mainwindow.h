#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include "sm_subnetwidget.h"
#include <QItemSelectionModel>
#include <QTableView>
#include "sm_datamodel.h"
#include "sm_infodockwidget.h"
#include <QCheckBox>
#include <QList>
#include <QMenu>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSize sizeHint() const;

protected:
    void resizeEvent ( QResizeEvent * event );

private slots:
    void openFile(const QString &path = QString());
    void saveFile();

    void printFile();

    void showAboutDialog();

    void addIPv4Subnet();
    void addIPv6Subnet();

    void searchFieldChanged();
    void searchFieldCleared();

    void deleteCurrentSubnet();
    void editCurrentSubnet();
    void showInfoPane();

    void selectionChanged();

    void autoResizeClicked();
    void killAutoResize();

    void recentDocsMenuTrigger();


public slots:

    void mapWasAltered();

private:
    void resetTitle();
    void setupModel();
    void setupViews();

    void addRecentDocument(QString filename);
    void parseRecentDocuments();
    void generateRecentDocsMenu();

    QList<QString> recentDocs;
    SM_DataModel *model;
    SM_SubnetWidget *map;
    QItemSelectionModel *selectionModel;
    QTableView *table;
    QLineEdit *searchField;
    SM_InfoDockWidget *infoDock;
    QCheckBox *autoResizeOption;
    QMenu *recentDocsMenu;

    bool changedButNotSaved;

};

#endif // MAINWINDOW_H
