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

#include "session.h"
#include "space.h"
#include "itemdata.h"
#include "manager.h"

#include <QWebSettings>

Session::Session(Manager *manager, QObject *parent)
    : QObject(parent)
{
    // setup
    m_manager = manager;

    /**
    Webkit settings from Qt API, each line corresponds to a value between 0 and 14

    0 - QWebSettings::AutoLoadImages                    Specifies whether images are automatically loaded in web pages.
    1 - QWebSettings::JavascriptEnabled                 Enables or disables the running of JavaScript programs.
    2 - QWebSettings::JavaEnabled	                Enables or disables Java applets. Currently Java applets are not supported.
    3 - QWebSettings::PluginsEnabled	                Enables or disables plugins in Web pages.
    4 - QWebSettings::PrivateBrowsingEnabled	        Private browsing prevents WebKit from recording visited pages in the history and storing web page icons.
    5 - QWebSettings::JavascriptCanOpenWindows	        Specifies whether JavaScript programs can open new windows.
    6 - QWebSettings::JavascriptCanAccessClipboard	Specifies whether JavaScript programs can read or write to the clipboard.
    7 - QWebSettings::DeveloperExtrasEnabled	        Enables extra tools for Web developers. Currently this enables the "Inspect" element in the context menu, which shows the WebKit WebInspector for web site debugging.
    8 - QWebSettings::LinksIncludedInFocusChain         Specifies whether hyperlinks should be included in the keyboard focus chain.
    9 - QWebSettings::ZoomTextOnly	                Specifies whether the zoom factor on a frame applies to only the text or all content.
    10- QWebSettings::PrintElementBackgrounds	        Specifies whether the background color and images are also drawn when the page is printed.
    11- QWebSettings::OfflineStorageDatabaseEnabled	Specifies whether support for the HTML 5 offline storage feature is enabled or not.
    12- QWebSettings::OfflineWebApplicationCacheEnabled Specifies whether support for the HTML 5 web application cache feature is enabled or not.
    13- QWebSettings::LocalStorageDatabaseEnabled	Specifies whether support for the HTML 5 local storage feature is enabled or not.
    14- QWebSettings::AllowUniversalAccessFromFileUrls  Specifies whether documents from file Urls should be granted universal access (e.g., to HTTP and HTTPS documents).
    **/    

    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(0), true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(1), true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(2), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(3), true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(4), true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(5), true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(6), true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(7), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(8), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(9), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(10), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(11), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(12), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(13), false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::WebAttribute(14), true);
}

Session::~Session()
{
    // free resources that have no parent
    foreach (Space *space, spaces()) {
        delete space;
    }

    foreach (ItemData *data, dataItems()) {
        delete data;
    }
}

// Public functions

void Session::setIdentity(const QString username, const QString password)
{
    m_username = username;
    m_password = password;
}

QString Session::username() const
{
    return m_username;
}

QString Session::password() const
{
    return m_password;
}

void Session::setSkey(const QString key)
{
    m_skey = key;
}

QString Session::skey() const
{
    return m_skey;
}

QList<Space*> Session::spaces() const
{
    return m_spaces;
}

Space* Session::space(const qulonglong id) const
{
    foreach(Space* space, m_spaces) {
        if (space->id() == id)
            return space;
    }
    return NULL;
}

void Session::addSpace(const qulonglong id, const QString name)
{
    Space *new_space = new Space(m_manager, id, name);
    m_spaces.append(new_space);
}

QList<ItemData*> Session::dataItems() const
{
    return m_dataItems;
}

ItemData* Session::dataItem(const qulonglong id) const
{
    foreach(ItemData* data, m_dataItems) {
        if (data->id() == id)
            return data;
    }
    return NULL;
}

ItemData* Session::dataItem(const QString name) const
{
    foreach(ItemData* data, m_dataItems) {
        if (data->name() == name)
            return data;
    }
    return NULL;
}

void Session::addItem(qulonglong id, QString name, QString url, qulonglong width, qulonglong height,
                      QPixmap picture)
{
    ItemData *new_data = new ItemData(id, name, url, width, height, picture);
    m_dataItems.append(new_data);
}

void Session::clear()
{
    m_username = "";
    m_password = "";
    m_skey = "";
    int i = 0;
    foreach(ItemData *data, m_dataItems) {
        delete data;
        m_dataItems.removeAt(i++);
    }
    i = 0;
    foreach(Space *space, m_spaces) {
        delete space;
        m_spaces.removeAt(i++);
    }
}
