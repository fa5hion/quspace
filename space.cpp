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

#include "space.h"
#include "spaceitem.h"
#include "webwidget.h"
#include "manager.h"
#include "itemdata.h"
#include "taskbar.h"

#include <cstdlib>
#include <QTime>
#include <QSettings>
#include <QDebug>

Space::Space(Manager *manager, qulonglong id, QString name, QWidget *parent)
    : QGraphicsView(parent)
{
    // setup
    m_manager = manager;
    m_id = id;
    m_name = name;
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::HighQualityAntialiasing, true);
    setScene(new QGraphicsScene());
    setMouseTracking(true);
    // ... background
    QString bg_stylesheet = "QGraphicsView { background: qlineargradient(x1: 0,"
                            "                                            y1: 0,"
                            "                                            x2: 0,"
                            "                                            y2: 1,"
                            "                                            stop: 0 black,"
                            "                                            stop: 1 gray);"
                            "}"
                            "QScrollBar:vertical {"
                            "border: none;"
                            "background: #575757;"
                            "width: 15px;"
                            "margin: 15px 0px 15px 0px;"
                            "}"
                            "QScrollBar::sub-line:vertical {"
                            "image: url(:/up_arrow);"
                            "height: 15px;"
                            "subcontrol-position: top;"
                            "subcontrol-origin: margin;"
                            "}"
                            "QScrollBar::add-line:vertical {"
                            "image: url(:/down_arrow);"
                            "height: 15px;"
                            "subcontrol-position: bottom;"
                            "subcontrol-origin: margin;"
                            "}"
                            "QScrollBar:horizontal {"
                            "border: none;"
                            "background: #575757;"
                            "width: 15px;"
                            "margin: 0px 15px 0px 15px;}"
                            "QScrollBar::sub-line:horizontal {"
                            "image: url(:/left_arrow);"
                            "height: 15px;"
                            "subcontrol-position: left;"
                            "subcontrol-origin: margin;"
                            "}"
                            "QScrollBar::add-line:horizontal {"
                            "image: url(:/right_arrow);"
                            "height: 15px;"
                            "subcontrol-position: right;"
                            "subcontrol-origin: margin;"
                            "}";
    this->setStyleSheet(bg_stylesheet);
    // ... spacebar
    QSettings settings;
    m_taskbar = new TaskBar(m_manager, this, QGraphicsView::scene(), this);
}

Space::~Space()
{
    // Free resources
    foreach (SpaceItem *item, items()) {
        delete item;
    }
}

// Public functions

qulonglong Space::id() const
{
    return m_id;
}

QString Space::name() const
{
    return m_name;
}

QList<SpaceItem*> Space::items() const
{
    return m_items;
}

QRectF Space::boundingRect() const
{
    // stub
    return QRectF(0, 0, 0, 0);
}

TaskBar* Space::taskbar() const
{
    return m_taskbar;
}

bool Space::isTaskbarVisible() const
{
    if (m_taskbar->isVisible())
        return true;
    else
        return false;
}

void Space::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // stub
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    return;
}

// Public slots

void Space::addItem(const qulonglong id, const QString name, const QString url,
                    const qulonglong width, const qulonglong height, const QPixmap picture)
{
    SpaceItem *item = new SpaceItem(QGraphicsView::scene(), id, name, url, width, height, picture, this);
    m_items.append(item);
    item->setZValue(m_items.count());
    // TODO: implement more intelligent positioning
    qsrand(QTime(0, 0, 0).msecsTo(QTime::currentTime()));
    int x = (geometry().width() - width) * (qrand()/float(RAND_MAX));
    qsrand(QTime(0, 0, 0).msecsTo(QTime::currentTime()));
    int y = (geometry().height() - height) * (qrand()/float(RAND_MAX));
    // move item to pos(x,y)
    item->setPos(x, y);
    // signal-slot connections
    connect(item, SIGNAL(itemClicked()), this, SLOT(raiseItem()));
    connect(item, SIGNAL(itemRemoveAsked()), this, SLOT(removeItem()));
    connect(item, SIGNAL(itemMaximizeAsked()), this, SLOT(maximizeItem()));
    connect(item, SIGNAL(itemMinimizeAsked()), this, SLOT(minimizeItem()));
}

void Space::raiseItem(SpaceItem *item)
{
    if (!item && sender())
        item = qobject_cast<SpaceItem*>(sender());
    if (Manager::DEBUG) qDebug() << "Selected: " << item->widget()->title();
    if (item) {
        // ensure that widget is set on top of others
        item->setZValue(1000);
        item->setRaised();
        int index_old = m_items.indexOf(item, 0);
        // rearrange items
        m_items.move(index_old, m_items.size() - 1);
        int zValue = 1;
        // init new zValues (stacking)
        foreach(SpaceItem *item_tmp, m_items) {
            item_tmp->setZValue(zValue);
            if (zValue != m_items.size())
                item_tmp->setLowered();
            zValue++;
        }
    }
}

void Space::removeItem(SpaceItem *item)
{
    if (!item && sender())
        item = qobject_cast<SpaceItem*>(sender());
    if (item) {
        // remove widget form user profile in uspace system
        m_manager->requestWidgetRemove(id(), item->data()->id());
        // ... and from view
        QGraphicsView::scene()->removeItem(item);
        for (int i=0; i<m_items.size(); i++) {
            if (m_items.at(i) == item) {
                m_items.removeAt(i);
                if (Manager::DEBUG) qDebug() << "Removed: " << item->widget()->title();
                break;
            }
        }
        // free up
        item->deleteLater();
    }
}

void Space::maximizeItem(SpaceItem *item)
{
    if (!item && sender())
        item = qobject_cast<SpaceItem*>(sender());
    if (item) {
        // request: item size = space size
        item->handleResize(QRectF(0, 0, rect().width(), rect().height()), true);
    }
}

void Space::minimizeItem(SpaceItem *item)
{
    if (!item && sender())
        item = qobject_cast<SpaceItem*>(sender());
    if (item) {
        item->setVisible(false);
        m_taskbar->itemMinimized(item);
    }
}

void Space::restoreItem(SpaceItem *item)
{
    if (!item && sender())
        item = qobject_cast<SpaceItem*>(sender());
    if (item) {
        item->setVisible(true);
        raiseItem(item);
    }
}

// Protected functions

void Space::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::scene()->setSceneRect(0, 0, e->size().width(), e->size().height());
    m_taskbar->reposition();
}
