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

#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QGraphicsItem>
#include <QGraphicsWebView>

class WebWidget : public QGraphicsWebView
{
    Q_OBJECT

public:
    WebWidget(const QString url, const qulonglong width, const qulonglong height, QGraphicsItem *parent = 0);
    ~WebWidget();
    void setWidgetRect(const QRectF rect);
    QRectF widgetRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget = 0);

public slots:
    void handleLoadFinished();
    void handleLoadStarted();
    void handleRefresh();

private:
    QRectF m_widgetRect;
    QPixmap m_widgetPix;
    QPixmap m_loadingPix;
    QPixmap m_gripPix;
    QSize m_pageSize;
    bool m_isLoading;
};

#endif // WEBWIDGET_H
