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

#ifndef FRAMEEMOJI_H
#define FRAMEEMOJI_H

#include <QFrame>
#include "listwidgetemoji.h"
#include "emojimanager.h"

namespace Ui {
class FrameEmoji;
}

class FrameEmoji : public QFrame
{
    Q_OBJECT
    
public:
    explicit FrameEmoji(QWidget *parent = 0, QString emojiPath = "");
    ~FrameEmoji();
    
private:
    Ui::FrameEmoji *ui;
    EmojiManager *emojiMan;
    QString m_emojiPath;

    void loadComboCategories();

private slots:   
    void loadEmojiCategory(QString category);
    void emojiDoubleClickReceived(QString symbol);

signals:
    void sendEmoji(QString);
};

#endif // FRAMEEMOJI_H
