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

#ifndef TOOLSCOMPONENT_H
#define TOOLSCOMPONENT_H

#include <QWidget>

namespace Ui {
    class ToolsComponent;
}

class Manager;

class ToolsComponent : public QWidget
{
    Q_OBJECT

public:
    ToolsComponent(Manager *manager, QWidget *parent = 0);
    ~ToolsComponent();
    void resetLabels();
    void informWidgetTab(const QString msg);

public slots:
    void handleWidgetAdd();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ToolsComponent *ui;
    Manager *m_manager;
};

#endif // TOOLSCOMPONENT_H
