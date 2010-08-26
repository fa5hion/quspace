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

#include "toolscomponent.h"
#include "ui_toolscomponent.h"
#include "itemdata.h"
#include "manager.h"
#include "session.h"

ToolsComponent::ToolsComponent(Manager *manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolsComponent)
{
    // setup
    ui->setupUi(this);
    m_manager = manager;
    // ... background
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, this->sizeHint().height()));
    linearGrad.setColorAt(0, Qt::gray);
    linearGrad.setColorAt(1, Qt::white);
    QPalette palette;
    palette.setBrush(QPalette::Window, linearGrad);
    setPalette(palette);
    ui->textBrowser->setText("Textual description...");
    // TODO: implement icon fetching.
    QIcon test_icon = QIcon(QPixmap(":/test"));
    // add available items to dialog
    foreach (ItemData *data, m_manager->session()->dataItems()) {
        new QListWidgetItem(test_icon, data->name(), ui->itemList);
    }
    // signal-slot connections
    connect(ui->addWidgetButton, SIGNAL(clicked()), this, SLOT(handleWidgetAdd()));
}

ToolsComponent::~ToolsComponent()
{
    delete ui;
}

// Public functions
void ToolsComponent::resetLabels()
{
    ui->addWidgetInfo->setText("Add a widget to currently active space.");
}

void ToolsComponent::informWidgetTab(const QString msg)
{
    ui->addWidgetInfo->setText(msg);
}

// Public slots

void ToolsComponent::handleWidgetAdd()
{
    QListWidgetItem *item = ui->itemList->currentItem();
    if (!item) {
        ui->addWidgetInfo->setText("Select a widget first.");
        return;
    }
    ui->addWidgetInfo->setText("Waiting for uSpace to process the request.");
    m_manager->requestWidgetAdd(item->text());
}

// Protected functions

void ToolsComponent::changeEvent(QEvent *e)
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
