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

#ifndef TASKBAR_H
#define TASKBAR_H

#include <QGraphicsWidget>
#include <QGraphicsScene>

class SpaceItem;
class Space;
class ItemEventfilter;
class Manager;

class TaskBar : public QGraphicsWidget
{
    Q_OBJECT

public:
    TaskBar(Manager *manager, Space *space, QGraphicsScene *scene, QGraphicsItem *parent = 0);
    ~TaskBar();
    void doShow();
    void doHide();
    void reposition();
    void itemMinimized(SpaceItem *item);
    void toggle();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget = 0);

public slots:
    void handleTaskbarClick(const QPointF position);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
    Manager *m_manager;
    Space *m_space;
    QRectF m_rectSize;
    int m_size;
    int m_bottomPad;
    int m_controlPad1;
    int m_controlPad2;
    int m_controlSize;
    int m_iconPad;
    int m_iconSize;
    ItemEventfilter *m_filter;
    QList<SpaceItem*> m_items;
    int m_itemsA;
    int m_itemsB;
};

#endif // TASKBAR_H
