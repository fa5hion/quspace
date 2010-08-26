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

#include "logincomponent.h"
#include "ui_logincomponent.h"
#include "manager.h"

LoginComponent::LoginComponent(Manager *manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginComponent)
{
    // setup
    ui->setupUi(this);
    m_manager = manager;
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setMaxLength(50);
    ui->usernameEdit->setMaxLength(50);
    // ... validator for username
    QRegExp rx("[a-zA-Z0-9_.@]{1,50}");
    QValidator *nameValidator = new QRegExpValidator(rx, ui->usernameEdit);
    ui->usernameEdit->setValidator(nameValidator);
    // signal-slot connections
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(sendLoginRequest()));
}

LoginComponent::~LoginComponent()
{
    delete ui;
    // TODO: free resources
}

// Public slots

void LoginComponent::initialize(const QString msg)
{
    ui->usernameEdit->setText(emptyString());
    ui->passwordEdit->setText(emptyString());

    if (msg == "") {
        setInfoColor(Qt::white);
        ui->infoLabel->setText("Login with uSpace or OtaSizzle account.");
    }
    else {
        setInfoColor(Qt::red);
        ui->infoLabel->setText(msg);
    }
    ui->usernameEdit->setFocus();
}

void LoginComponent::sendLoginRequest()
{    
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if (!validUsername(username)) {
        if (username == "")
            initialize("Error: empty username given.");
        else
            initialize("Error: illegal username, " + username + ".");
    }
    else if (!validPassword(password))
        initialize("Error: illegal password.");
    else {
        m_manager->requestLogin(username, password);
    }
}

void LoginComponent::handleLoginReply(const QString msg)
{
    initialize(msg);
}

// Protected functions

void LoginComponent::changeEvent(QEvent *e)
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

// Private functions

bool LoginComponent::validUsername(QString username)
{
    const QValidator *validator = ui->usernameEdit->validator();
    int pos;
    if ((validator->validate(username, pos) != QValidator::Acceptable) || (username.size() < 4))
        return false;
    return true;
}

bool LoginComponent::validPassword(QString password)
{
    if (password.size() < 4)
        return false;
    return true;
}

QString LoginComponent::emptyString()
{
    QString retval;
    return retval;
}

void LoginComponent::setInfoColor(QColor color)
{
    QPalette infoPalette(ui->infoLabel->palette());
    infoPalette.setColor(QPalette::WindowText, color);
    ui->infoLabel->setPalette(infoPalette);
}
