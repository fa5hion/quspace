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
#include "logincomponent.h"
#include "workspacecomponent.h"
#include "toolscomponent.h"
#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QSettings>

static const QSize RESOLUTION(1024, 600);
static const QString URL_PREFIX = "http://testbed.tml.hut.fi/uspace/rest";
static const bool DEBUG = false;
static const bool LOCAL_WIDGETS = false;

int main(int argc, char *argv[])
{
    // setup
    QCoreApplication::setOrganizationName("aalto_uni");
    QCoreApplication::setApplicationName("quspace");
    QSettings settings;
    if (!settings.contains("misc/debug"))
        settings.setValue("misc/debug", DEBUG);
    if (!settings.contains("network/url_prefix"))
        settings.setValue("network/url_prefix", URL_PREFIX);
    QApplication a(argc, argv);
    // ... screen resolution
    QSize resolution;
    if (argc != 3) {
        if (!settings.contains("screen/resolution")) {
            resolution = QSize(RESOLUTION.width(), RESOLUTION.height());
            settings.setValue("screen/resolution", resolution);
        }
        else
            resolution = settings.value("screen/resolution").value<QSize>();
    }
    else {
        resolution = QSize(QString::fromAscii(argv[1]).toInt(),
                           QString::fromAscii(argv[2]).toInt());
        settings.setValue("screen/resolution", resolution);
    }
    // ... manager
    Manager *manager = new Manager();
    // ... UI components
    MainWindow *window = new MainWindow(manager, resolution);
    LoginComponent *login = new LoginComponent(manager, window);
    WorkspaceComponent *workspace = new WorkspaceComponent(manager, window);
    window->setComponent(login);
    window->setComponent(workspace);
    manager->setUIComponent(window);
    manager->setUIComponent(login);
    manager->setUIComponent(workspace);
    // fix for windows build
    window->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    // show main window
    window->show();
    // execute event loop
    return a.exec();
}
