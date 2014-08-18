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

#include "sm_aboutdialog.h"
#include "ui_sm_aboutdialog.h"

SM_AboutDialog::SM_AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SM_AboutDialog)
{
    ui->setupUi(this);
    ui->versionLabel->setText(tr("Version ")+qApp->applicationVersion());
}

SM_AboutDialog::~SM_AboutDialog()
{
    delete ui;
}
