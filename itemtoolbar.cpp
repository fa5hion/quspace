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

#include "itemtoolbar.h"
#include "spaceitem.h"

#include <QGraphicsProxyWidget>

ItemToolbar::ItemToolbar(SpaceItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags)
{
    setParentItem(parent);
    // setup buttons
    //const int s_title_pad = SpaceItem::s_title_pad;
    //const int s_side_pad = SpaceItem::s_side_pad;
    // stylesheet for TRANSPARENT button
    QString b_stylesheet = "QToolButton { margin: 0px;"
                           "              border: 0px;"
                           "              padding: 0px;"
                           "              width: 16px;"
                           "              height: 16px;"
                           "              background-color: rgba(255,255,255,0); }";
    // ... min
    m_minButton = new QToolButton();
    m_minButton->setIcon(QIcon(QPixmap(":/window_min").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    m_minButton->setToolTip(tr("Minimize Widget"));
    m_minButton->setAutoRaise(true);
    m_minButton->setStyleSheet(b_stylesheet);
    // ... max
    m_maxButton = new QToolButton();
    m_maxButton->setIcon(QIcon(QPixmap(":/window_max").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    m_maxButton->setToolTip(tr("Maximize Widget"));
    m_maxButton->setAutoRaise(true);
    m_maxButton->setStyleSheet(b_stylesheet);
    // ... close
    m_closeButton = new QToolButton();
    m_closeButton->setIcon(QIcon(QPixmap(":/window_close").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    m_closeButton->setToolTip(tr("Remove Widget"));
    m_closeButton->setAutoRaise(true);
    m_closeButton->setStyleSheet(b_stylesheet);
    // layout
    m_layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    m_layout->setContentsMargins(3, 3, 3, 3);
    m_layout->setItemSpacing(0, 2);
    m_layout->setItemSpacing(1, 2);
    m_layout->addItem(parent->scene()->addWidget(m_minButton));
    m_layout->addItem(parent->scene()->addWidget(m_maxButton));
    m_layout->addItem(parent->scene()->addWidget(m_closeButton));
    setLayout(m_layout);
    m_layout->activate();
    // signal-slot connections
    connect(m_minButton, SIGNAL(clicked()), parent, SLOT(handleMinButtonClicked()));
    connect(m_maxButton, SIGNAL(clicked()), parent, SLOT(handleMaxButtonClicked()));
    connect(m_closeButton, SIGNAL(clicked()), parent, SLOT(handleCloseButtonClicked()));
}

ItemToolbar::~ItemToolbar()
{
    // free resources that have no parent
    delete m_closeButton;
    delete m_minButton;
    delete m_maxButton;
}

void ItemToolbar::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
    // suppress compiler warnings
    Q_UNUSED(painter);
    Q_UNUSED(options);
    Q_UNUSED(widget);
    // TODO: do we need something here?
}
