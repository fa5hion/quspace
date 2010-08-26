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

#ifndef SPACE_H
#define SPACE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QResizeEvent>

class SpaceItem;
class TaskBar;
class Manager;

class Space : public QGraphicsView, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem);

public:
    Space(Manager *manager, qulonglong id, QString name, QWidget *parent = 0);
    ~Space();
    qulonglong id() const;
    QString name() const;
    QList<SpaceItem*> items() const;
    QRectF boundingRect() const;
    TaskBar* taskbar() const;
    bool isTaskbarVisible() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *options, QWidget *widget = 0);

public slots:
    void addItem(const qulonglong id, const QString name, const QString url,
                 const qulonglong width, const qulonglong height, const QPixmap picture);
    void raiseItem(SpaceItem *item = 0);
    void removeItem(SpaceItem *item = 0);
    void maximizeItem(SpaceItem *item = 0);
    void minimizeItem(SpaceItem *item = 0);
    void restoreItem(SpaceItem *item = 0);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    TaskBar *m_taskbar;
    Manager *m_manager;
    qulonglong m_id;
    QString m_name;
    QList<SpaceItem*> m_items;
};

#endif // SPACE_H
