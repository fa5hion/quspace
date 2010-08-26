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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class Manager;
class LoginComponent;
class WorkspaceComponent;
class ToolsComponent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Manager *manager, const QSize windowSize, QWidget *parent = 0);
    ~MainWindow();
    void showLogin();
    void showWorkspace();
    void showTools();
    void createTools();
    void setLoadingCursor();
    void setNormalCursor();
    void setComponent(QWidget *component);

public slots:
    void handleExit();
    void handleBackButtonClicked();
    void handleToolsButtonClicked();
    void handleTaskbarToggle();
    void handleTaskbarIconUpdate();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    Manager *m_manager;
    LoginComponent *m_login;
    WorkspaceComponent *m_workspace;
    ToolsComponent *m_tools;
    QWidget *m_activeComponent;
    QSize m_windowSize;
    void setTaskbarIconMode(bool on);
};

#endif // MAINWINDOW_H
