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

#include "workspacecomponent.h"
#include "ui_workspacecomponent.h"
#include "manager.h"
#include "space.h"
#include "taskbar.h"

#include <QToolButton>
#include <QDebug>

WorkspaceComponent::WorkspaceComponent(Manager *manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkspaceComponent)
{
    // setup
    ui->setupUi(this);
    // prevent annoying focus frame from showing up.
    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    m_manager = manager;
}

WorkspaceComponent::~WorkspaceComponent()
{
    delete ui;
}

// Public functions

qulonglong WorkspaceComponent::activeSpaceId() const
{
    Space *space = qobject_cast<Space*>(ui->tabWidget->currentWidget());
    if (space != 0)
        return (space->id());
    else
        return 0;
}

// Public slots

void WorkspaceComponent::addSpaceTab(Space *space)
{
    // add to tabwidget
    ui->tabWidget->addTab(space, space->name());
    // prevent annoying focus frame from showing up.
    int index_of_new_tab = ui->tabWidget->count() - 1;
    ui->tabWidget->widget(index_of_new_tab)->setFocusPolicy(Qt::NoFocus);
    // TODO: change font
    //ui->tabWidget->widget(index_of_new_tab)->setFont(QFont("fixed", 16, 75, false));
    // close button
    QToolButton *closeButton = new QToolButton();
    closeButton->setIcon(QIcon(QPixmap(":/window_close").scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    closeButton->setToolTip(tr("Remove Space"));
    closeButton->setAutoRaise(true);
    // ... stylesheet
    QString b_stylesheet = "QToolButton { margin: 0px;"
                           "              border: 0px;"
                           "              padding: 0px;"
                           "              width: 22px;"
                           "              height: 22px;"
                           "              background-color: rgba(255,255,255,0); }";
    closeButton->setStyleSheet(b_stylesheet);
    // ... insert button
    QTabBar *bar = ui->tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));
    bar->setTabButton(index_of_new_tab, QTabBar::RightSide, closeButton);
    if (index_of_new_tab == 0)
        bar->setTabsClosable(true);
    // signal-slot connections
    connect(closeButton, SIGNAL(clicked()), this, SLOT(removeSpaceTab()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SIGNAL(tabSwitched()));
}

void WorkspaceComponent::removeSpaceTab()
{
    // TODO: is this done right?
    // TODO: ask for confirmation.
    int index = ui->tabWidget->currentIndex();
    ui->tabWidget->widget(index)->close();
    ui->tabWidget->removeTab(index);
}

void WorkspaceComponent::removeSpaceTabs()
{
    // remove all
    int num_of_tabs = ui->tabWidget->count();
    for (int i=num_of_tabs-1; i>-1; i--) {
        ui->tabWidget->widget(i)->deleteLater();
        ui->tabWidget->removeTab(i);
    }
}

// Protected functions

void WorkspaceComponent::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
