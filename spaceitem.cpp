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

#include "spaceitem.h"
#include "webwidget.h"
#include "itemtoolbar.h"
#include "itemeventfilter.h"
#include "itemdata.h"
#include "space.h"

#include <QWebFrame>

const int SpaceItem::s_title_pad = 22; // padding on the top of the widget
const int SpaceItem::s_side_pad = 1; // padding on the other three sides
const bool SpaceItem::SHADOW = false;

SpaceItem::SpaceItem(QGraphicsScene *scene, const qulonglong id, const QString name, const QString url,
                     const qulonglong width, const qulonglong height, const QPixmap picture, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
    // setup
    m_data = new ItemData(id, name, url, width, height, picture, this);
    m_maxed = false;
    scene->addItem(this);
    // ... item size
    QRectF itemRect(0, 0, width + 2*s_side_pad, height + s_title_pad + 2*s_side_pad);
    setBoundingRect(itemRect);
    setGeometry(geometry().x(), geometry().y(), itemRect.width(), itemRect.height());
    m_backgroundBrush = palette().background();
    m_titleBrush = palette().foreground();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    // ... webwidget
    m_widget = new WebWidget(url, width, height, this);
    m_widget->setPos(s_side_pad, s_title_pad + s_side_pad);
    // ... toolbar
    m_toolBar = new ItemToolbar(this);
    m_toolBar->setPos(boundingRect().topRight() - QPointF(m_toolBar->rect().width(), 0));
    // ... shadow effect
    if (SHADOW) {
        m_shadow = new QGraphicsDropShadowEffect(this);
        m_shadow->setProperty("offset", QPointF(3, 3));
        m_shadow->setProperty("blurRadius", 3);
        setGraphicsEffect(m_shadow);
    }
    // ... event filter
    m_filter = new ItemEventfilter(m_widget, this);
    // signal-slot connections
    connect(m_filter, SIGNAL(resized(const QRectF)), this, SLOT(handleResize(const QRectF)));
    connect(m_filter, SIGNAL(routeClick()), this, SLOT(handleClickRouting()));
    // start as lowered
    setLowered();
}

SpaceItem::~SpaceItem()
{

}

ItemData* SpaceItem::data() const
{
    return m_data;
}

void SpaceItem::setBoundingRect(const QRectF rect)
{
    m_rect = rect;
}

QRectF SpaceItem::boundingRect() const
{
    return m_rect;
}

WebWidget* SpaceItem::widget() const
{
    return m_widget;
}

void SpaceItem::setRaised()
{
    m_titleBrush = Qt::black;
    if (SHADOW)
        m_shadow->setColor(Qt::black);
}

void SpaceItem::setLowered()
{
    m_titleBrush = Qt::gray;
    if (SHADOW)
        m_shadow->setColor(Qt::gray);
}

void SpaceItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // suppress compiler warning
    Q_UNUSED(widget);
    Q_UNUSED(option);
    // help measurements
    int w = boundingRect().width();
    int h = boundingRect().height();
    int x = boundingRect().x();
    int y = boundingRect().y();
    //int fontHeight = painter->fontMetrics().height();
    // draw frame for space item
    painter->setPen(Qt::SolidLine);
    painter->setBrush(m_backgroundBrush);
    painter->drawRoundedRect(x, y, w, h, 5, 0);
    // draw titlebar gradient
    QLinearGradient linearGrad(x, y , x, s_title_pad);
    linearGrad.setColorAt(0, Qt::gray);
    linearGrad.setColorAt(1, Qt::white);
    painter->setPen(Qt::NoPen);
    painter->setBrush(linearGrad);
    // ... with two rectangles: upper rounded, lower normal
    painter->drawRoundedRect(x + s_side_pad,
                             y + s_side_pad,
                             w - 2*s_side_pad,
                             s_title_pad - 5,
                             5, 0);
    painter->drawRect(x + s_side_pad,
                      y + s_side_pad + 5,
                      w - 2*s_side_pad,
                      s_title_pad - 5);
    QFont font;
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);
    // form elided version of title string
    QString title = painter->fontMetrics().elidedText(data()->name(),
                                                      Qt::ElideRight,
                                                      w - 2*s_side_pad - m_toolBar->rect().width());
    painter->setPen(QPen(m_titleBrush, 0));
    painter->setBrush(Qt::NoBrush);
    painter->drawText(5, 5, w - 5 - 2*s_side_pad, s_title_pad - 5, Qt::AlignLeft, title);
}

void SpaceItem::handleMinButtonClicked()
{
    //emit itemClicked();
    emit itemMinimizeAsked();

}

void SpaceItem::handleMaxButtonClicked()
{
    emit itemClicked();
    if (!m_maxed) {
        // save geometry
        m_oldGeometry = geometry();
        // request maximize
        emit itemMaximizeAsked();
        m_maxed = true;
    }
    else {
        m_maxed = false;
        // restore geometry
        setGeometry(m_oldGeometry);
        // resize accordingly
        handleResize(QRectF(0,
                            0,
                            m_oldGeometry.width() - 2*s_side_pad,
                            m_oldGeometry.height() - s_title_pad - 2*s_side_pad));
    }
}

void SpaceItem::handleCloseButtonClicked()
{
    // remove without confirmation for now
    // TODO: confirm dialog?
    emit itemRemoveAsked();
}

void SpaceItem::handleResize(const QRectF rect, const bool maximize)
{
    // set webwidget's news size
    m_widget->setWidgetRect(rect);
    // set item's new size
    QRectF itemRect(0, 0, rect.width() + 2*s_side_pad, rect.height() + s_title_pad + 2*s_side_pad);
    setBoundingRect(itemRect);
    // set new geometry
    if (maximize)
        setGeometry(0, 0, itemRect.width(), itemRect.height());
    else
        setGeometry(geometry().x(), geometry().y(), itemRect.width(), itemRect.height());
    // put toolbar into correct position
    m_toolBar->setPos(boundingRect().topRight() - QPointF(m_toolBar->rect().width(), 0));
    //update();
}

void SpaceItem::handleClickRouting()
{
    emit itemClicked();
}

// Protected functions

void SpaceItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit itemClicked();
    QGraphicsWidget::mousePressEvent(event);
}

void SpaceItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseReleaseEvent(event);
}
