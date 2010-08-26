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

#ifndef MANAGER_H
#define MANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class MainWindow;
class LoginComponent;
class WorkspaceComponent;
class ToolsComponent;
class Session;
class Space;
class SpaceItem;
class QSettings;

class Manager : public QObject
{
    Q_OBJECT

public:
    Manager(QObject *parent = 0);
    ~Manager();
    void setUIComponent(QWidget *component);
    void requestLogin(const QString username, const QString password);
    void requestSpaces();
    void requestWidgets(const qulonglong spaceID);
    void requestAllWidgets();
    void requestWidgetAdd(const QString name);
    void requestWidgetRemove(const qulonglong spaceID, const qulonglong widgetID);
    Session* session() const;
    void routeTaskbarChange();
    static bool DEBUG;
    static bool LOCAL_WIDGETS;

public slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QString m_urlprefix;
    Session *m_session;
    QNetworkAccessManager *m_network;
    MainWindow *m_window;
    LoginComponent *m_login;
    WorkspaceComponent *m_workspace;
    ToolsComponent *m_tools;
    QString parseReplyMessage(const QByteArray &input, const QString type) const;
    bool parseSkey(const QByteArray &input) const;
    bool parseSpaces(const QByteArray &input) const;
    bool parseWidgets(const QByteArray &input, const qulonglong spaceID) const;
};

#endif // MANAGER_H
