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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manager.h"
#include "session.h"
#include "logincomponent.h"
#include "workspacecomponent.h"
#include "space.h"
#include "toolscomponent.h"
#include "taskbar.h"

#include <QResizeEvent>

MainWindow::MainWindow(Manager *manager, const QSize windowSize, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // setup
    ui->setupUi(this);
    m_manager = manager;
    m_login = 0;
    m_workspace = 0;
    m_tools = 0;
    // ... splitter
    QString spl_stylesheet = "QSplitter::handle { image: url(:/grip_side); height: 16px; }";
    ui->splitter->setStyleSheet(spl_stylesheet);
    // ... element sizes and positions
    m_windowSize = windowSize;
    setGeometry(0, 0, m_windowSize.width(), m_windowSize.height());
    // ... background
    QString bg_stylesheet = "QMainWindow { background: qlineargradient(x1: 0,"
                            "                                          y1: 0,"
                            "                                          x2: 0,"
                            "                                          y2: 1,"
                            "                                          stop: 0 gray,"
                            "                                          stop: 1 white); }";
    this->setStyleSheet(bg_stylesheet);
    // signal-slot connections
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(handleExit()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(handleBackButtonClicked()));
    connect(ui->toolsButton, SIGNAL(clicked()), this, SLOT(handleToolsButtonClicked()));
    connect(ui->taskbarButton, SIGNAL(clicked()), this, SLOT(handleTaskbarToggle()));
}

MainWindow::~MainWindow()
{
    delete ui;
    // TODO: free resources
}

// Public functions

void MainWindow::setComponent(QWidget *component)
{
    if (component->objectName() == "LoginComponent") {
        m_login = qobject_cast<LoginComponent*>(component);
        ui->leftPlaceHolder->addWidget(m_login, 0, 0);
    }
    else if (component->objectName() == "WorkspaceComponent") {
        m_workspace = qobject_cast<WorkspaceComponent*>(component);
        ui->leftPlaceHolder->addWidget(m_workspace, 0, 0);
        connect(m_workspace, SIGNAL(tabSwitched()), this, SLOT(handleTaskbarIconUpdate()));
    }
    // both initial components setup -> show login
    if (m_login!=0 && m_workspace!=0)
        showLogin();
}

void MainWindow::showLogin()
{
    ui->backButton->hide();
    ui->toolsButton->hide();
    ui->taskbarButton->hide();
    if (m_tools)
        m_tools->hide();
    if (m_workspace)
        m_workspace->hide();
    if (m_login) {
        m_login->show();
        m_login->initialize("");
        m_activeComponent = m_login;
    }
}

void MainWindow::showWorkspace()
{
    QString btn_stylesheet;
    if (m_activeComponent->objectName() == "ToolsComponent") {
        m_tools->hide();
        ui->toolsButton->show();
        ui->taskbarButton->show();
        ui->quitButton->show();
    }
    else if (m_activeComponent->objectName() == "LoginComponent") {
        m_login->hide();
        ui->backButton->show();
        ui->toolsButton->show();
        ui->taskbarButton->show();
    }
    m_workspace->show();
    m_activeComponent = m_workspace;
}

void MainWindow::showTools()
{
    // shouldn't happen...
    if (m_activeComponent->objectName() == "LoginComponent")
        return;
    // null pointer...
    if (m_tools == 0)
        return;
    m_workspace->hide();
    ui->toolsButton->hide();
    ui->taskbarButton->hide();
    ui->quitButton->hide();
    m_tools->resetLabels();
    m_tools->show();
    m_activeComponent = m_tools;
}

void MainWindow::createTools()
{
    m_tools = new ToolsComponent(m_manager, this);
    m_tools->hide();
    ui->leftPlaceHolder->addWidget(m_tools, 0, 0);
}

void MainWindow::setLoadingCursor()
{
    m_activeComponent->setCursor(Qt::WaitCursor);
}

void MainWindow::setNormalCursor()
{
    m_activeComponent->setCursor(Qt::ArrowCursor);
}

// Public slots

void MainWindow::handleExit()
{
    if (m_activeComponent->objectName() == "WorkspaceComponent") {
        m_manager->session()->clear();
    }
    close();
}

void MainWindow::handleBackButtonClicked()
{
    if (m_activeComponent->objectName() == "WorkspaceComponent") {
        m_manager->session()->clear();
        showLogin();
    }
    else if (m_activeComponent->objectName() == "ToolsComponent")
        showWorkspace();
}

void MainWindow::handleToolsButtonClicked()
{
    if (m_activeComponent->objectName() == "WorkspaceComponent")
        showTools();
}

void MainWindow::handleTaskbarToggle()
{
    qulonglong id = m_workspace->activeSpaceId();
    if (!id)
        return;
    m_manager->session()->space(id)->taskbar()->toggle();
    if (m_manager->session()->space(id)->taskbar()->isVisible()) {
        setTaskbarIconMode(true);
    }
    else {
        setTaskbarIconMode(false);
    }
}

void MainWindow::handleTaskbarIconUpdate()
{
    qulonglong id = m_workspace->activeSpaceId();
    if (!id)
        return;
    if (m_manager->session()->space(id)->taskbar()->isVisible()) {
        setTaskbarIconMode(true);
    }
    else {
        setTaskbarIconMode(false);
    }
}

// Protected functions

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// Private functions

void MainWindow::setTaskbarIconMode(bool on)
{
    QString btn_stylesheet;
    if (on) {
        btn_stylesheet = "border: 0px;"
                         "background: rgb(255,255,255,0);"
                         "image: url(:/taskbar_icon_visible);";
    }
    else {
        btn_stylesheet = "border: 0px;"
                         "background: rgb(255,255,255,0);"
                         "image: url(:/taskbar_icon_invisible);";
    }
    ui->taskbarButton->setStyleSheet(btn_stylesheet);
}
