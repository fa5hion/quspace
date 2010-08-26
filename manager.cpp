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

#include "manager.h"
#include "mainwindow.h"
#include "logincomponent.h"
#include "workspacecomponent.h"
#include "toolscomponent.h"
#include "session.h"
#include "space.h"
#include "itemdata.h"
#include "parser.h"

#include <QRegExp>
#include <QRegExpValidator>
#include <QSettings>
#include <QDebug>

bool Manager::DEBUG = false;
bool Manager::LOCAL_WIDGETS = false;

Manager::Manager(QObject *parent)
    : QObject(parent)
{
    // setup
    m_window = 0;
    m_login = 0;
    m_workspace = 0;
    m_tools = 0;
    m_network = new QNetworkAccessManager(this);
    QSettings settings;
    // load settings from local registry
    if (settings.contains("misc/debug"))
        DEBUG = settings.value("misc/debug").value<bool>();
    m_urlprefix = settings.value("network/url_prefix").value<QString>();
    // signal-slot connections
    connect(m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleNetworkReply(QNetworkReply*)));
}

Manager::~Manager()
{
    delete m_network;
}

// Public functions

void Manager::setUIComponent(QWidget *component)
{
    if (component->objectName() == "MainWindow") {
        m_window = qobject_cast<MainWindow*>(component);
        // ensure memory is freed when window is deleted
        this->setParent(m_window);
    }
    else if (component->objectName() == "LoginComponent")
        m_login = qobject_cast<LoginComponent*>(component);
    else if (component->objectName() == "WorkspaceComponent")
        m_workspace = qobject_cast<WorkspaceComponent*>(component);
}

void Manager::requestLogin(const QString username, const QString password)
{
    m_session = new Session(this, this);
    m_session->setIdentity(username, password);
    // Debug with local widgets
    if (LOCAL_WIDGETS) {
        m_window->showWorkspace();
        qulonglong id = 100;
        qulonglong size = 170;
        m_session->setSkey("00000001");
        m_session->addSpace(id, "Local test space 1");
        m_session->addSpace(id + 1, "Local test space 2");
        foreach (Space *space, m_session->spaces()) {
            m_workspace->addSpaceTab(space);
            m_session->space(space->id())->addItem(1, "Clock 1", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(2, "Clock 2", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(3, "Clock 2", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(4, "Clock 3", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(5, "Clock 4", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(6, "Clock 5", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(7, "Clock 6", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(8, "Clock 7", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(9, "Clock 8", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(10, "Clock 9", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(11, "Clock 10", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(12, "Clock 11", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
            m_session->space(space->id())->addItem(13, "Clock 12", "http://localhost/clock.html", size, size,
                                                   QPixmap(":/test"));
        }
        return;
    }
    QNetworkRequest request;
    // RFC 2617 pages 4-5
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(username).arg(password).toAscii()).toBase64());
    request.setUrl(QUrl(m_urlprefix + "/login?username=" + username + "&password=" + password));
    m_network->get(request);
}

void Manager::requestSpaces()
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(m_session->username()).arg(m_session->password()).toAscii()).toBase64());
    request.setUrl(QUrl(m_urlprefix + "/spaces?skey=" + m_session->skey()));
    m_network->get(request);
}

void Manager::requestWidgets(const qulonglong spaceID)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(m_session->username()).arg(m_session->password()).toAscii()).toBase64());
    // convert from qulonglong to QString
    QString spaceID_s;
    spaceID_s.setNum(spaceID);
    request.setUrl(QUrl(m_urlprefix + "/widgets?spaceid=" + spaceID_s + "&skey=" + m_session->skey()));
    m_network->get(request);
}

void Manager::requestAllWidgets()
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(m_session->username()).arg(m_session->password()).toAscii()).toBase64());
    request.setUrl(QUrl(m_urlprefix + "/allwidgets?skey=" + m_session->skey()));
    m_network->get(request);
}

void Manager::requestWidgetAdd(const QString name)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(m_session->username()).arg(m_session->password()).toAscii()).toBase64());
    QByteArray bytes;
    QString spaceID_s;
    spaceID_s.setNum(m_workspace->activeSpaceId());
    QString widgetID_s;
    widgetID_s.setNum(m_session->dataItem(name)->id());
    if (DEBUG) qDebug() << "Add: Widget (" << widgetID_s << ") to Space (" << spaceID_s << ")";
    request.setUrl(QUrl(m_urlprefix + "/addwidgettospace?skey=" + m_session->skey() + "&spaceid=" + spaceID_s + "&widgetid=" + widgetID_s));
    m_network->post(request, bytes);
}

void Manager::requestWidgetRemove(const qulonglong spaceID, const qulonglong widgetID)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(m_session->username()).arg(m_session->password()).toAscii()).toBase64());
    QByteArray bytes;
    QString spaceID_s;
    spaceID_s.setNum(spaceID);
    QString widgetID_s;
    widgetID_s.setNum(widgetID);
    if (DEBUG) qDebug() << "Remove: Widget (" << widgetID << ") from Space (" << spaceID << ")";
    request.setUrl(QUrl(m_urlprefix + "/removewidgetfromspace?skey=" + m_session->skey() + "&spaceid=" + spaceID_s + "&widgetid=" + widgetID));
    m_network->post(request, bytes);
}

Session* Manager::session() const
{
    return m_session;
}

void Manager::routeTaskbarChange()
{
    m_window->handleTaskbarIconUpdate();
}

// Public slots

void Manager::handleNetworkReply(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();
    QString str(data);

    if (reply->error() > 0) {
        qDebug() << "ERROR in network reply: " << str;
        return;
    }
    // Validating patterns
    // TODO: these need tuning!
    QRegExp skey_rx("[{]{1}[\"]{1}[s]{1}[k]{1}[e]{1}[y]{1}.*");
    QRegExp spaces_rx("[{]{1}[\"]{1}[s]{1}[p]{1}[a]{1}[c]{1}[e]{1}[s]{1}.*");
    QRegExp widgets_rx("[{]{1}[\"]{1}[w]{1}[i]{1}[d]{1}[g]{1}[e]{1}[t]{1}[s]{1}.*");
    QRegExp error_rx("[{]{1}[\"]{1}[e]{1}[r]{1}[r]{1}[o]{1}[r]{1}.*");
    QRegExp success_rx("[{]{1}[\"]{1}[s]{1}[u]{1}[c]{1}[c]{1}[e]{1}[s]{1}[s]{1}.*");
    QRegExpValidator skey_v(skey_rx, 0);
    QRegExpValidator spaces_v(spaces_rx, 0);
    QRegExpValidator widgets_v(widgets_rx, 0);
    QRegExpValidator error_v(error_rx, 0);
    QRegExpValidator success_v(success_rx, 0);

    int pos;

    // SKEY reply
    if (skey_v.validate(str, pos)) {
        // inform login widget about success
        m_login->handleLoginReply("Successfully logged in as " + m_session->username() + ".");
        if (DEBUG) qDebug() << "SKEY: " << str;
        if (parseSkey(data)) {
            // got the session key, get available widgets in the system
            requestAllWidgets();
            // ... and user's spaces
            requestSpaces();
        }
    }
    // SPACES reply
    else if (spaces_v.validate(str, pos)) {
        if (DEBUG) qDebug() << "SPACES: " << str;
        if (parseSpaces(data)) {
            // got valid spaces data, switch to workspace
            m_window->showWorkspace();
            // show spaces on workspace and get widgets for each space
            foreach (Space *space, m_session->spaces()) {
                m_workspace->addSpaceTab(space);
                requestWidgets(space->id());
            }
        }
    }
    // WIDGETS OR ALLWIDGETS reply
    else if (widgets_v.validate(str, pos)) {
        qulonglong spaceID = reply->request().url().queryItemValue("spaceid").toULongLong();
        if (spaceID != 0) {
            if (DEBUG) qDebug() << "WIDGETS IN SPACE (" << spaceID << "): " << str;
        }
        else {
            if (DEBUG) qDebug() << "WIDGETS IN SYSTEM: " << str;
        }
        parseWidgets(data, spaceID);
    }
    // SUCCESS reply
    else if (success_v.validate(str, pos)) {
        if (DEBUG) qDebug() << "SUCCESS: " << str;
        QString msg = parseReplyMessage(data, "success");
        if (msg == "Widget was successfully added to the space") {
            qulonglong spaceID = reply->request().url().queryItemValue("spaceid").toULongLong();
            qulonglong id = reply->request().url().queryItemValue("widgetid").toULongLong();
            QString name = m_session->dataItem(id)->name();
            QString url = parseReplyMessage(data, "widgetUrl");
            if (DEBUG) qDebug() << "URL of new widget: " << url;
            qulonglong width = m_session->dataItem(id)->width();
            qulonglong height = m_session->dataItem(id)->height();
            m_session->space(spaceID)->addItem(id, name, url, width, height, QPixmap(":/test"));
            m_window->showWorkspace();
        }
        else if (msg == "Widget was successfully added to the space") {
            qulonglong spaceID = reply->request().url().queryItemValue("spaceid").toULongLong();
            qulonglong id = reply->request().url().queryItemValue("widgetid").toULongLong();
            if (DEBUG) qDebug() << "Widget (" << id << ") removed from space (" << spaceID << ")";
        }
        else {
            qDebug() << "Unknown success reply: " << str;
        }
    }
    // ERROR reply
    else if (error_v.validate(str, pos)) {
        if (DEBUG) qDebug() << "ERROR: " << str;
        m_login->handleLoginReply("Error: " + parseReplyMessage(data, "error") + ".");
    }
    // UNKNOWN reply
    else {
        if (DEBUG) qDebug() << "UNKNOWN REPLY: " << str;
        m_login->handleLoginReply("Error: unknown network error.");
    }
}

// Private functions

QString Manager::parseReplyMessage(const QByteArray &input, QString type) const
{
    // qjson parsing engine in external library
    QJson::Parser p;
    bool parse_success = false;
    QString reply_str = p.parse(input, &parse_success).toMap()[type].toString();
    if (parse_success)
        return reply_str;
    return "Unknown error";
}

bool Manager::parseSkey(const QByteArray &input) const
{
    // qjson parsing engine in external library
    QJson::Parser p;
    bool parse_success = false;
    QString skey = p.parse(input, &parse_success).toMap()["skey"].toString();
    if (parse_success) {
        // save session key to local session storage
        m_session->setSkey(skey);
        return true;
    }
    return false;
}

bool Manager::parseSpaces(const QByteArray &input) const
{
    // qjson parsing engine in external library
    QJson::Parser p;
    bool parse_success = false;
    QVariantList spaces = p.parse(input, &parse_success).toMap()["spaces"].toList();
    if (parse_success) {
        for (int i=0; i<spaces.size(); i++) {
            qulonglong spaceID = spaces[i].toMap()["id"].toULongLong();
            QString name = spaces[i].toMap()["name"].toString();
            // add container for space to local session storage
            m_session->addSpace(spaceID, name);
        }
        return true;
    }
    return false;
}

bool Manager::parseWidgets(const QByteArray &input, const qulonglong spaceID) const
{
    // qjson parsing engine in external library
    QJson::Parser p;
    bool parse_success = false;
    QVariantList widgets = p.parse(input, &parse_success).toMap()["widgets"].toList();
    if (parse_success) {
        for (int i=0; i<widgets.size(); i++) {
            qulonglong id = widgets[i].toMap()["id"].toULongLong();
            QString name = widgets[i].toMap()["name"].toString();
            QString url = widgets[i].toMap()["url"].toString();
            qulonglong width = widgets[i].toMap()["width"].toULongLong();
            qulonglong height = widgets[i].toMap()["height"].toULongLong();
            if (spaceID == 0) {
                // parsing system widgets, save to session
                if (DEBUG) qDebug() << "ADD ( space" << spaceID << "): " << name << " : " << width << "x" << height << " : " << url;
                m_session->addItem(id, name, url, width, height, QPixmap(":/test"));
            }
            else {
                // parsing active widgets, add to space
                if (DEBUG) qDebug() << "ADD ( space" << spaceID << "): " << name << " : " << width << "x" << height << " : " << url;
                m_session->space(spaceID)->addItem(id, name, url, width, height, QPixmap(":/test"));
            }
        }
        // Need to wait for system widgets before creating tools.
        if (!m_tools)
            m_window->createTools();
        return true;
    }
    return false;
}
