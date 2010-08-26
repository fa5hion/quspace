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

#include "webwidget.h"
#include "spaceitem.h"

#include <QWebFrame>

WebWidget::WebWidget(const QString url, const qulonglong width, const qulonglong height, QGraphicsItem *parent)
    : QGraphicsWebView(parent)
    , m_isLoading(true)
{
    // setup
    QRect rect(SpaceItem::s_side_pad, SpaceItem::s_title_pad, width, height);
    setWidgetRect(rect);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // ... widget content
    setUrl(url);
    // ... image that is shown while loading widget content
    m_loadingPix = QPixmap(":/arrow_down").scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // ... image for window grip
    m_gripPix = QPixmap(":/grip_bottomright");
    // ... page object properties
    page()->setPalette(QPalette(Qt::white));
    page()->setViewportSize(rect.size());
    page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAsNeeded);
    page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAsNeeded);
    // signal - slot connections
    connect(this, SIGNAL(loadStarted()), this, SLOT(handleLoadStarted()));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(handleLoadFinished()));
}

WebWidget::~WebWidget()
{
    // TODO: free resources
}

// Public functions

void WebWidget::setWidgetRect(const QRectF rect)
{
    m_widgetRect = rect;
    setGeometry(QRectF(pos(), rect.size()));
    if (!m_isLoading)
        page()->mainFrame()->setScrollPosition(QPoint(rect.x(), rect.y()));
}

QRectF WebWidget::widgetRect() const
{
    return m_widgetRect;
}

void WebWidget::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget)
{
    int w = boundingRect().width();
    int h = boundingRect().height();
    int x = boundingRect().x();
    int y = boundingRect().y();
    // draw widget edges
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    // ... with two rectangles: upper normal, lower rounded
    painter->drawRect(x, y, w, h - 5);
    painter->drawRoundedRect(x, y + 5, w, h - 5, 5, 5);

    if (m_isLoading) {
        // page loading, paint existing content and loading pixmap
        painter->setOpacity(1.0);
        painter->drawPixmap(0,
                            0,
                            m_widgetPix);
        painter->drawPixmap((w / 2) - (m_loadingPix.width() / 2),
                            (h / 2) - (m_loadingPix.height() / 2),
                            m_loadingPix);
    }
    else {
        painter->save();
        QGraphicsWebView::paint(painter, options, widget);
        painter->restore();
        // draw window grip
        painter->setOpacity(1.0);
        painter->drawPixmap(w - 16, h - 16, m_gripPix);
    }
}

// Public slots

void WebWidget::handleLoadStarted()
{
    m_isLoading = true;
    setCursor(Qt::WaitCursor);
}

void WebWidget::handleLoadFinished()
{
    page()->mainFrame()->setScrollPosition(QPoint(widgetRect().x(), widgetRect().y()));
    //parentItem()->update();
    update();
    setCursor(Qt::ArrowCursor);
    m_isLoading = false;
}

void WebWidget::handleRefresh()
{
    // save widget contents
    m_widgetPix = QPixmap(QSize(widgetRect().width(), widgetRect().height()));
    m_widgetPix.fill(Qt::transparent);
    QPainter painter(&m_widgetPix);
    page()->mainFrame()->render(&painter);
    if (!m_isLoading)
        reload();
}
