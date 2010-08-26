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

#include "itemeventfilter.h"

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsWebView>
#include <QGraphicsWidget>
#include <QDebug>

ItemEventfilter::ItemEventfilter(QGraphicsItem *item, QObject *parent)
    : QObject(parent)
{
    m_item = item;
    m_rect = item->boundingRect();
    if (dynamic_cast<QGraphicsWebView*>(item))
        m_gripSize = QSizeF(20, 20);
    else if (dynamic_cast<QGraphicsWidget*>(item)) {
        m_gripSize = QSizeF(0, 0);
    }
    m_cursor_normal = Qt::ArrowCursor;
    m_cursor_resize = Qt::SizeFDiagCursor;
    m_resizing = false;
    item->scene()->addItem(this);
    item->installSceneEventFilter(this);
    // needed for cursor change
    item->setAcceptHoverEvents(true);
}

ItemEventfilter::~ItemEventfilter()
{
    // TODO: free resources
}

// Public functions

QRectF ItemEventfilter::gripRect() const {
    // center the grip to the bottom right corner
    QPointF pos = m_rect.bottomRight() - QPointF(m_gripSize.width() / 2, m_gripSize.height() / 2);
    return QRectF(pos, m_gripSize);
}

QRectF ItemEventfilter::resize(QRectF oldRect, const QPointF oldPoint, const QPointF newPoint)
{
    // calculate new rect size
    qreal dx1 = 0;
    qreal dx2 = newPoint.x() - oldPoint.x();
    qreal dy1 = 0;
    qreal dy2 = newPoint.y() - oldPoint.y();
    QRectF newRect = oldRect.adjusted(dx1, dy1, dx2, dy2);
    if (m_rect != newRect) {
        m_rect = newRect;
        // signal spaceitem::handleResize
        emit resized(newRect);
    }
    return newRect;
}

bool ItemEventfilter::sceneEventFilter(QGraphicsItem *watched, QEvent* event)
{
    // suppress compiler warning
    Q_UNUSED(watched);

    QPointF eventPos;

    QGraphicsSceneHoverEvent *hoverEvent = dynamic_cast<QGraphicsSceneHoverEvent*>(event);
    QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if (!hoverEvent && !mouseEvent)
        return false;

    if (hoverEvent)
        eventPos = hoverEvent->pos();
    if (mouseEvent)
        eventPos = mouseEvent->pos();

    if (mouseEvent && (m_rect.width() == 0)) {
        if (event->type() != QEvent::GraphicsSceneMouseRelease)
            emit taskbarClicked(eventPos);
        return true;
    }

    if (event->type() == QEvent::GraphicsSceneMouseRelease) {
        m_resizing = false;
        return false;
    }

    // at GRIP
    if (gripRect().contains(eventPos)) {
        // change to resize cursor above grip
        if (event->type() == QEvent::GraphicsSceneHoverMove) {
            m_item->setCursor(m_cursor_resize);
            return true;
        }
        // start resize within grip
        else if (event->type() == QEvent::GraphicsSceneMousePress) {
            m_resizeStartPos = eventPos;
            m_resizeStartRect = m_rect;
            m_resizing = true;
            return true;
        }
    }
    // outside GRIP
    else {
        // change to normal cursor
        if (event->type() == QEvent::GraphicsSceneHoverMove) {
            m_item->setCursor(m_cursor_normal);
            return true;
        }
        // item selected
        else if (event->type() == QEvent::GraphicsSceneMousePress) {
            emit routeClick();
            return false;
        }
    }

    // resize
    if (m_resizing && (event->type() == QEvent::GraphicsSceneMouseMove)) {
        resize(m_resizeStartRect, m_resizeStartPos, eventPos);
        return true;
    }

    return false;
}

// Function stubs below are needed to implement QGraphicsItem class.
// Avoid crash: pure virtual function call

QRectF ItemEventfilter::boundingRect() const
{
    return m_rect;
}

void ItemEventfilter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)   // dummy
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
