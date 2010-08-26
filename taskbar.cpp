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

#include "taskbar.h"
#include "itemeventfilter.h"
#include "space.h"
#include "spaceitem.h"
#include "itemdata.h"
#include "manager.h"

TaskBar::TaskBar(Manager *manager, Space *space, QGraphicsScene *scene, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
    // setup
    m_manager = manager;
    m_space = space;
    m_size = 5;
    m_itemsA = 0;
    m_itemsB = 0;
    m_bottomPad = 10;
    m_controlPad1 = 7;
    m_controlPad2 = 13;
    m_controlSize = 32;
    m_iconPad = 5;
    m_iconSize = 48;
    m_rectSize = QRectF(0,
                        0,
                        2 * (m_controlPad1 + m_controlPad2) + 2 * m_controlSize + m_size * m_iconSize + m_size * (m_size - 1),
                        m_iconSize + 2 * m_iconPad);
    scene->addItem(this);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    doHide();
    // ... event filter
    m_filter = new ItemEventfilter(this, this);
    // signal-slot connections
    connect(m_filter, SIGNAL(taskbarClicked(QPointF)), this, SLOT(handleTaskbarClick(QPointF)));
}

TaskBar::~TaskBar()
{

}

// Public functions

void TaskBar::doShow()
{
    setVisible(true);
    // ensure always on top
    setZValue(10000);
}

void TaskBar::doHide()
{
    setVisible(false);
}

void TaskBar::reposition()
{
    QRectF new_geometry((m_space->size().width() - m_rectSize.width()) / 2,
                        m_space->size().height() - m_rectSize.height() - m_bottomPad,
                        m_rectSize.width(),
                        m_rectSize.height());
    setGeometry(new_geometry);
}

void TaskBar::itemMinimized(SpaceItem *item)
{
    m_items.append(item);
    m_itemsB += 1;
    if ((m_itemsB - m_itemsA) > m_size) {
        m_itemsA += m_size;
    }
    doShow();
    update();
    // first item added, light taskbar indicator
    if (m_items.size() > 0)
        m_manager->routeTaskbarChange();
}

void TaskBar::toggle()
{
    if (isVisible())
        doHide();
    else
        doShow();
}

void TaskBar::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // suppress compiler warning
    Q_UNUSED(widget);
    Q_UNUSED(option);
    // help measurements
    int w = boundingRect().width();
    int h = boundingRect().height();
    int x = boundingRect().x();
    int y = boundingRect().y();
    // draw frame for taskbar
    painter->setPen(QPen(QColor("#656565"), 3, Qt::SolidLine));
    painter->setBrush(QColor(Qt::black));
    painter->drawRoundedRect(x, y, w, h, 3, 3);
    // draw controls
    painter->setOpacity(1.0);
    painter->drawPixmap(x + m_controlPad1,
                        y + ((m_rectSize.height() - m_controlSize) / 2),
                        QPixmap(":/sign_left").scaled(m_controlSize, m_controlSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter->drawPixmap(x + w - m_controlPad1 - m_controlSize,
                        y + ((m_rectSize.height() - m_controlSize) / 2),
                        QPixmap(":/sign_right").scaled(m_controlSize, m_controlSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // draw widget icons
    int xx = x + m_controlPad1 + m_controlSize + m_controlPad2;
    int yy = y + m_iconPad;
    for (int i=m_itemsA; i<m_itemsB; i++) {
        if (m_items.size() == 0)
            return;
        QPixmap icon(m_items.at(i)->data()->picture().scaled(m_iconSize, m_iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        painter->drawPixmap(xx, yy, icon);
        xx += (m_iconSize + m_iconPad);
    }
}

// Public slots

void TaskBar::handleTaskbarClick(QPointF position)
{
    // rects for icon collision detection
    QList<QRectF> iconRects;
    int x = m_controlPad1 + m_controlSize + m_controlPad2;
    int y = m_iconPad;
    for (int i=0; i<m_size; i++) {
        iconRects.append(QRectF(QPointF(x, y), QSizeF(m_iconSize, m_iconSize)));
        x += m_iconPad + m_iconSize;
    }
    // check if click position hits anyone
    for (int i=0; i<m_size; i++) {
        if (i > (m_itemsB - m_itemsA)) {
            break;
        }
        if (iconRects.at(i).contains(position)) {
            m_space->restoreItem(m_items.at(m_itemsA + i));
            m_items.removeAt(m_itemsA + i);
            if (((m_itemsB - 1) % m_size) == 0)
                m_itemsA -= m_size;
            m_itemsB = m_itemsA + m_size;
            if (m_itemsB > m_items.size())
                m_itemsB = m_items.size();
            update();
            // empty taskbar, hide
            if (m_items.size() == 0) {
                doHide();
                m_manager->routeTaskbarChange();
            }
            return;
        }
    }

    // rects for control collision detection
    QList<QRectF> controlRects;
    x = m_controlPad1;
    y = (m_rectSize.height() - m_controlSize) / 2;
    for (int i=0; i<2; i++) {
        controlRects.append(QRectF(QPointF(x, y), QSizeF(m_controlSize, m_controlSize)));
        x += m_rectSize.width() - m_controlPad1 - m_controlSize;
    }
    // check if click position hits anyone
    for (int i=0; i<2; i++) {
        if (controlRects.at(i).contains(position)) {
            // left hit
            if (i == 0) {
                if (m_itemsA == 0) {
                    return;
                }
                else {
                    m_itemsA -= m_size;
                    m_itemsB = m_itemsA + m_size;
                }
            }
            // right hit
            else if (i == 1) {
                if (m_itemsB == m_items.size()) {
                    return;
                }
                else {
                    m_itemsA += m_size;
                    m_itemsB += m_size;
                    if (m_itemsB > m_items.size())
                        m_itemsB = m_items.size();
                }
            }
            update();
        }
    }
}

// Protected functions

void TaskBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mousePressEvent(event);
}

void TaskBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseReleaseEvent(event);
}

