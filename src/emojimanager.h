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

#ifndef EMOJI_H
#define EMOJI_H

#include <QObject>
#include <QList>
#include <QMap>

class Emoji
{
public:
    QString symbol;
    QString description;
    QString pixmapIconNumber;
    QString category;
};

class EmojiCategory
{
public:
    QString categoryName;
    QString iconNumber;
};

class EmojiManager : public QObject
{
    Q_OBJECT
public:
    explicit EmojiManager(QObject *parent = 0);

    QList<Emoji> getEmojiList();
    QList<EmojiCategory> getEmojiCategoryList();
    QString getEmojiNumberFromSymbol(QString symbol);
    QString getEmojiSymbolFromNumber(QString number);

private:
    QStringList readText(QString fileName);
    void parseEmoticonList(QStringList content);
    
    QList<Emoji> emojiList;
    QList<EmojiCategory> categoryList;
    QMap<QString, QString> symbolNumberMap;
    QMap<QString, QString> numberSymbolMap;

signals:
    
public slots:
    
};

#endif // EMOJI_H
