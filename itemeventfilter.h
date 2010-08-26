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

#ifndef ITEMEVENTFILTER_H
#define ITEMEVENTFILTER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QCursor>

class ItemEventfilter : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    ItemEventfilter(QGraphicsItem *item, QObject *parent = 0);
    ~ItemEventfilter();
    QRectF gripRect() const;
    QRectF resize(QRectF originalRect, const QPointF originalPoint, const QPointF newPoint);
    bool sceneEventFilter(QGraphicsItem* watched, QEvent* event);
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

signals:
    void resized(const QRectF &rect);
    void routeClick();
    void taskbarClicked(QPointF position);

private:
    QGraphicsItem *m_item;
    bool m_resizing;
    QRectF m_rect;
    QSizeF m_gripSize;
    QCursor m_cursor_normal;
    QCursor m_cursor_resize;
    QPointF m_resizeStartPos;
    QRectF m_resizeStartRect;
};

#endif // ITEMEVENTFILTER_H
