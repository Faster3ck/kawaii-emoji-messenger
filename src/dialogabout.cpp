/*
* This file is part of Kawaii Emoji Messenger, a multiplatform Facebook client
* based on the idea of bringing emoji on desktop.
*
* (C) Francesco Mondello 2014
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Faster <faster3ck@gmail.com>
*
*/

#include <QDesktopServices>
#include <QUrl>
#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "globals.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    QString nameVersion = QString("<span style=\"font-size:12pt;\"><strong>%1 - %2</strong></span>")
            .arg(globals::PROGRAM_NAME)
            .arg(globals::VERSION);

    ui->labelNameVersion->setText(nameVersion);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushDonate_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=BPS56LV66S6JS"));
}

void DialogAbout::on_pushOk_clicked()
{
    close();
}
