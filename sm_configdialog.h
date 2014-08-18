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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SM_CONFIGDIALOG_H
#define SM_CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class SM_ConfigDialog;
}

class SM_ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SM_ConfigDialog(QWidget *parent = 0);
    ~SM_ConfigDialog();

public slots:

    void accept();

private slots:
    void on_sn_autoloadBrowse_clicked();

    void on_sn_clearRecent_clicked();

private:
    Ui::SM_ConfigDialog *ui;
};

#endif // SM_CONFIGDIALOG_H
