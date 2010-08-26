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

#ifndef SESSION_H
#define SESSION_H

#include <QList>
#include <QObject>
#include <QDebug>

class Space;
class Manager;
class ItemData;
class QPixmap;

class Session : public QObject
{
    Q_OBJECT

public:
    Session(Manager *manager, QObject *parent = 0);
    ~Session();
    void setIdentity(const QString username, const QString password);
    QString username() const;
    QString password() const;
    void setSkey(const QString key);
    QString skey() const;
    QList<Space*> spaces() const;
    Space* space(const qulonglong id) const;
    void addSpace(const qulonglong id, const QString name);
    QList<ItemData*> dataItems() const;
    ItemData* dataItem(const qulonglong id) const;
    ItemData* dataItem(const QString name) const;
    void addItem(const qulonglong id, const QString name, const QString url,
                 const qulonglong width, const qulonglong height, const QPixmap picture);
    void clear();

private:
    Manager *m_manager;
    QString m_username;
    QString m_password;
    QString m_skey;
    QList<Space*> m_spaces;
    QList<ItemData*> m_dataItems;
};

#endif // SESSION_H
