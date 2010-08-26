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

#ifndef SPACEITEM_H
#define SPACEITEM_H

#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>

class WebWidget;
class ItemToolbar;
class ItemEventfilter;
class ItemData;

class SpaceItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    const static int s_side_pad, s_title_pad;
    const static bool SHADOW;

    SpaceItem(QGraphicsScene *scene, const qulonglong id, const QString name, const QString url,
              const qulonglong width, const qulonglong height, const QPixmap picture, QGraphicsItem *parent = 0);
    ~SpaceItem();
    ItemData* data() const;
    void setBoundingRect(const QRectF rect);
    QRectF boundingRect() const;
    WebWidget* widget() const;
    void setRaised();
    void setLowered();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget = 0);

signals:
    void itemClicked();
    void itemRemoveAsked();
    void itemMaximizeAsked();
    void itemMinimizeAsked();

public slots:
    void handleMinButtonClicked();
    void handleMaxButtonClicked();
    void handleCloseButtonClicked();
    void handleResize(const QRectF rect, const bool maximize = false);
    void handleClickRouting();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    ItemData *m_data;
    QRectF m_rect;
    QRectF m_oldGeometry;
    bool m_maxed;
    WebWidget *m_widget;
    ItemToolbar *m_toolBar;
    QBrush m_titleBrush;
    QBrush m_backgroundBrush;
    QGraphicsDropShadowEffect *m_shadow;
    ItemEventfilter *m_filter;
};

#endif // SPACEITEM_H
