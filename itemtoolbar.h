// Copyright (C) 2010 Jouni Mäenpää
// This file is part of QuSpace.

// QuSpace is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// QuSpace is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with QuSpace. If not, see <http://www.gnu.org/licenses/>.

#ifndef ITEMTOOLBAR_H
#define ITEMTOOLBAR_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QToolButton>

class SpaceItem;

class ItemToolbar : public QGraphicsWidget
{
    Q_OBJECT

public:
    ItemToolbar(SpaceItem *parent, Qt::WindowFlags wFlags = 0);
    ~ItemToolbar();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget = 0);

private:
    QGraphicsLinearLayout *m_layout;
    QToolButton *m_minButton;
    QToolButton *m_maxButton;
    QToolButton *m_closeButton;
};

#endif // ITEMTOOLBAR_H
