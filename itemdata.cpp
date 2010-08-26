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

#include "itemdata.h"

ItemData::ItemData(const qulonglong id, const QString name, const QString url, const qulonglong width, const qulonglong height,
                   const QPixmap picture, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
    , m_url(url)
    , m_width(width)
    , m_height(height)
    , m_picture(picture)
{

}

ItemData::~ItemData()
{

}

// Public functions

qulonglong ItemData::id() const
{
    return m_id;
}

QString ItemData::name() const
{
    return m_name;
}

QUrl ItemData::url() const
{
    return m_url;
}

qulonglong ItemData::width() const
{
    return m_width;
}

qulonglong ItemData::height() const
{
    return m_height;
}

QPixmap ItemData::picture() const
{
    return m_picture;
}
