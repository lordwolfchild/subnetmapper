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

#ifndef SM_SUBNETWIDGET_H
#define SM_SUBNETWIDGET_H

#include <QWidget>
#include "sm_modelbackend.h"
#include <QTimer>
#include <QHostInfo>

class SM_SubnetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SM_SubnetWidget(QWidget *parent = 0);
    ~SM_SubnetWidget();

    void setModelBackend(SM_ModelBackend *newmodel);

    bool searchHosts( QString name );

    void paintJob(QPainter *painter, QRect paintArea);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event );
    void mouseDoubleClickEvent(QMouseEvent *event );
    void clearCache();

signals:

public slots:

    void dataHasChanged();
    void selectionChangedInTable(const QModelIndex & current, const QModelIndex & previous);
    void selAnimTimerTriggered();

    void editCurrentSubnet();

    void xWidthPlus();
    void xWidthMinus();
    void line_heightPlus();
    void line_heightMinus();
    void upscale();

private:
    SM_ModelBackend *modelBackend;

    QList<QRect*> rectCache1_v4;
    QList<QRect*> rectCache2_v4;
    QList<QRect*> rectCache1_v6;
    QList<QRect*> rectCache2_v6;

    QList<QRect*> selectionCache;

    unsigned short selAnimState;
    QTimer *selAnimTimer;

    uint x_width;

    uint line_height;

    uint general_margin;

    QHostInfo searchedHosts;

};

#endif // SM_SUBNETWIDGET_H
