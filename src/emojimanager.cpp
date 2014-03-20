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

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "emojimanager.h"
#include "whereiam.h"

EmojiManager::EmojiManager(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_LINUX
    QDir dataLocation(WhereIAm::dataDir() + "/kawaii-emoji-messenger/data");
#else
    QDir dataLocation(WhereIAm::dataDir() + "/data");
#endif
    QString fileName = dataLocation.absolutePath() + "/emojilist.txt";

    QStringList listTmp = readText(fileName);

    // Insert firts category of emoji (ALL)
    EmojiCategory emojiCat;
    emojiCat.categoryName = "All";
    emojiCat.iconNumber = "093";

    categoryList << emojiCat;

    parseEmoticonList(listTmp);
}

QStringList EmojiManager::readText(QString fileName)
{
    QStringList content;
    
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QString line;
        do {
            line = in.readLine();
            content << line;
        } while (!line.isNull());
    }

    return content;
}

void EmojiManager::parseEmoticonList(QStringList content)
{
    QString currentCategory;    // currentCategory remembers the current category for every emoji!

    for (int i = 0; i < content.count(); i++) {
        QString line = content.at(i);

        if (line.isEmpty())
            break;

        if (line[0] == ';') {
            QStringList splitLine = line.split(" = ");

            currentCategory = splitLine.at(0);
            currentCategory = currentCategory.remove(";");

            EmojiCategory emojiCat;
            emojiCat.categoryName = currentCategory;
            emojiCat.iconNumber = splitLine.at(1);

            categoryList << emojiCat;
        }
        else {
            QStringList splitLine = line.split(" = ");

            Emoji emoji;

            emoji.symbol = splitLine.at(0);
            emoji.description = splitLine.at(1);
            emoji.pixmapIconNumber = splitLine.at(2);
            emoji.category = currentCategory;

            symbolNumberMap.insert(emoji.symbol, emoji.pixmapIconNumber);
            numberSymbolMap.insert(emoji.pixmapIconNumber, emoji.symbol);

            emojiList << emoji;

            /*qDebug() << "Symbol " << splitLine.at(0);
            qDebug() << "Name " << splitLine.at(1);
            qDebug() << "Number " << splitLine.at(2);
            qDebug() << "Category " << currentCategory;
            qDebug() << "------\n";*/
        }
    }
}

QList<Emoji> EmojiManager::getEmojiList()
{
    return emojiList;
}

QList<EmojiCategory> EmojiManager::getEmojiCategoryList()
{
    return categoryList;
}

QString EmojiManager::getEmojiNumberFromSymbol(QString symbol)
{
    return symbolNumberMap[symbol];
}

QString EmojiManager::getEmojiSymbolFromNumber(QString number)
{
    return numberSymbolMap[number];
}
