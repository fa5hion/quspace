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

#ifndef LOGINCOMPONENT_H
#define LOGINCOMPONENT_H

#include <QWidget>

namespace Ui {
    class LoginComponent;
}

class Manager;
class QValidator;

class LoginComponent : public QWidget
{
    Q_OBJECT

public:
    LoginComponent(Manager *manager, QWidget *parent = 0);
    ~LoginComponent();

public slots:
    void initialize(const QString msg);
    void sendLoginRequest();
    void handleLoginReply(const QString msg);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LoginComponent *ui;
    Manager *m_manager;
    bool validUsername(QString username);
    bool validPassword(QString password);
    QString emptyString();
    void setInfoColor(QColor color);
};

#endif // LOGINCOMPONENT_H
