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

#include "sm_configdialog.h"
#include "ui_sm_configdialog.h"
#include <QSettings>
#include <QFileDialog>

SM_ConfigDialog::SM_ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SM_ConfigDialog)
{
    ui->setupUi(this);

    QSettings settings;

    if (settings.value("mainwindow/autoload_map",0)==0) ui->sn_autoload->setChecked(false);
    else ui->sn_autoload->setChecked(true);
    ui->sn_autoloadPath->setText(settings.value("mainwindow/autoload_map_path","").toString());

    ui->sn_numRecent->setValue(settings.value("mainwindow/number_of_recent_docs",5).toInt());
    ui->sn_generalMargin->setValue(settings.value("ipv4widget/general_margin",30).toInt());
}

SM_ConfigDialog::~SM_ConfigDialog()
{
    delete ui;
}

void SM_ConfigDialog::accept()
{
    QSettings settings;

    settings.setValue("mainwindow/autoload_map_path",ui->sn_autoloadPath->text());
    if (ui->sn_autoload->isChecked()) settings.setValue("mainwindow/autoload_map",1);
    else settings.setValue("mainwindow/autoload_map",0);

    settings.setValue("ipv4widget/general_margin",ui->sn_generalMargin->value());
    settings.setValue("mainwindow/number_of_recent_docs",ui->sn_numRecent->value());

    QDialog::accept();
}

void SM_ConfigDialog::on_sn_autoloadBrowse_clicked()
{
    ui->sn_autoloadPath->setText(QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                            "", "*.smap"));
}

void SM_ConfigDialog::on_sn_clearRecent_clicked()
{
    QSettings settings;

    int max_docs=settings.value("mainwindow/number_of_recent_docs",5).toInt();

    for (int i=0;i<max_docs;i++) {
        settings.setValue("mainwindow/recent_doc_"+QString::number(i),"");
    }
}
