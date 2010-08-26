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

#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <QObject>
#include <QUrl>
#include <QPixmap>

class QPixmap;

class ItemData : public QObject
{
    Q_OBJECT

public:
    ItemData(const qulonglong id, const QString name, const QString url, const qulonglong width, const qulonglong height,
             const QPixmap picture, QObject *parent = 0);
    ~ItemData();
    qulonglong id() const;
    QString name() const;
    QUrl url() const;
    qulonglong width() const;
    qulonglong height() const;
    QPixmap picture() const;

private:
    qulonglong m_id;
    QString m_name;
    QUrl m_url;
    qulonglong m_width;
    qulonglong m_height;
    QPixmap m_picture;
};

#endif // ITEMDATA_H
