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

#ifndef SM_ABOUTDIALOG_H
#define SM_ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class SM_AboutDialog;
}

class SM_AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SM_AboutDialog(QWidget *parent = 0);
    ~SM_AboutDialog();

private:
    Ui::SM_AboutDialog *ui;
};

#endif // SM_ABOUTDIALOG_H
