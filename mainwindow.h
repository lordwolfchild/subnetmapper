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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include "sm_subnetwidget.h"
#include <QTableWidget>
#include <QTreeWidget>
#include <QTabWidget>
#include "sm_infodockwidget.h"
#include "sm_modelbackend.h"
#include <QCheckBox>
#include <QSpinBox>
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

public slots:
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
    void showConfigDialog();

    void selectionChanged();
    void modelDataHasChanged();

    void autoResizeClicked();
    void killAutoResize();

    void recentDocsMenuTrigger();

    void mapWasAltered();
    void updateSubnetTable();
    void updateIPv6Map();
    void subnetTableItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous);
    void subnetTableSelectRow(int index);

    void map6ItemClicked(QTreeWidgetItem * item, int column);

private:
    void resetTitle();
    void setupModel();
    void setupViews();

    QTreeWidgetItem* map6RecursivePopulator(QString prefix);

    void addRecentDocument(QString filename);
    void parseRecentDocuments();
    void generateRecentDocsMenu();

    QToolBar *toolbar;
    QToolBar *ipv4toolbar;
    QToolBar *ipv6toolbar;

    QTabWidget *tabArea;

    QList<QString> recentDocs;
    SM_ModelBackend *modelBackend;
    SM_SubnetWidget *map;
    QTreeWidget *map6;
    QTableWidget *table;
    QLineEdit *searchField;
    QSpinBox *ipv6Scale;
    SM_InfoDockWidget *infoDock;
    QCheckBox *autoResizeOption;
    QMenu *recentDocsMenu;

    bool changedButNotSaved;

};

#endif // MAINWINDOW_H
