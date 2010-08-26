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

#ifndef WORKSPACECOMPONENT_H
#define WORKSPACECOMPONENT_H

#include <QWidget>
#include <QHash>

namespace Ui {
    class WorkspaceComponent;
}

class Manager;
class Space;

class WorkspaceComponent : public QWidget
{
    Q_OBJECT

public:
    WorkspaceComponent(Manager *manager, QWidget *parent = 0);
    ~WorkspaceComponent();
    qulonglong activeSpaceId() const;

signals:
    void tabSwitched();
    void taskbarOpened();

public slots:
    void addSpaceTab(Space *space);
    void removeSpaceTab();
    void removeSpaceTabs();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WorkspaceComponent *ui;
    Manager *m_manager;
    QHash<qulonglong, QWidget*> m_tabs;
};

#endif // WORKSPACECOMPONENT_H
