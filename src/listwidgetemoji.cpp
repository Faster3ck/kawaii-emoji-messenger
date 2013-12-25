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

#include "listwidgetemoji.h"
#include "whereiam.h"

ListWidgetEmoji::ListWidgetEmoji(QWidget *parent) :
    QListWidget(parent)
{
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(getCurrentIndex(QListWidgetItem *)));
}

void ListWidgetEmoji::loadEmojiListView(QString category)
{
    m_emojiList = m_emojiMan->getEmojiList();

    for (int i = m_emojiList.count() - 1; i >= 0; i--) {
        if (category != "All") {
            if (category != m_emojiList.at(i).category)
                continue;
        }
        QListWidgetItem *newItem = new QListWidgetItem;

#ifdef Q_OS_LINUX
        QDir iconsLocation(WhereIAm::dataDir() + "/kawaii-emoji-messenger/data/icons");
#else
        QDir iconsLocation(WhereIAm::dataDir() + "/data/icons");
#endif
        QIcon ico;
        QString iconFileName = QString("%1/x-%2.png")
                .arg(iconsLocation.absolutePath())
                .arg(m_emojiList.at(i).pixmapIconNumber);
        QPixmap p(iconFileName);
        ico.addPixmap(p);
        newItem->setIcon(ico);

        newItem->setData(Qt::ToolTipRole, m_emojiList.at(i).description);

        insertItem(0, newItem);

        map.insert(m_emojiList.at(i).description, i);    // Description | Index
    }
}

void ListWidgetEmoji::setEmojiManager(EmojiManager *emojiMan)
{
    m_emojiMan = emojiMan;
}

void ListWidgetEmoji::getCurrentIndex(QListWidgetItem *item)
{
    QString descr = item->data(Qt::ToolTipRole).toString();
    int index = map[descr];    // Get the index using description

    emit sendEmoji(m_emojiList.at(index).symbol);
}
