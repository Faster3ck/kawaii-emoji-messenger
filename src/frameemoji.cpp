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

#include "frameemoji.h"
#include "ui_frameemoji.h"

FrameEmoji::FrameEmoji(QWidget *parent, QString emojiPath) :
    QFrame(parent),
    ui(new Ui::FrameEmoji)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup | Qt::Window);

    m_emojiPath = emojiPath;

    emojiMan = new EmojiManager();

    loadComboCategories();
    connect(ui->comboEmojiCategory, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadEmojiCategory(QString)));
    connect(ui->listWidgetEmoji, SIGNAL(sendEmoji(QString)), this, SLOT(emojiDoubleClickReceived(QString)));

    ui->listWidgetEmoji->setEmojiManager(emojiMan);
    ui->listWidgetEmoji->loadEmojiListView("All");

    //listWidget = ui->listWidgetEmoji;
}

FrameEmoji::~FrameEmoji()
{
    delete ui;
}

void FrameEmoji::loadComboCategories()
{
    QList<EmojiCategory> eCatList = emojiMan->getEmojiCategoryList();

    for (int i = 0; i < eCatList.count(); i++) {
        QString name = eCatList.at(i).categoryName;
        QString iconNumber = eCatList.at(i).iconNumber;

        QIcon ico;
        QString iconFileName = QString("%1/x-%2.png")
                .arg(m_emojiPath)
                .arg(iconNumber);
        QPixmap p(iconFileName);
        ico.addPixmap(p);

        ui->comboEmojiCategory->addItem(p, name);
    }
}

void FrameEmoji::loadEmojiCategory(QString category)
{
    ui->listWidgetEmoji->clear();
    ui->listWidgetEmoji->loadEmojiListView(category);
}

void FrameEmoji::emojiDoubleClickReceived(QString symbol)
{
    emit sendEmoji(symbol);
}
