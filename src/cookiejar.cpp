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

#include "cookiejar.h"
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QList>
#include <QDebug>
#include <QNetworkCookie>

CookieJar::CookieJar(QObject *parent, QString configPath) :
    QNetworkCookieJar(parent)
{
    m_configPath = configPath;
}

void CookieJar::save() {
    QString cachePath = m_configPath + "/cache";

    QList<QNetworkCookie> cookieList = allCookies();
    if(cookieList.isEmpty()) return;
    QDir configDir = QDir::home();
    if(!configDir.cd(cachePath)) {
        configDir.mkpath(QDir::cleanPath(cachePath));
        configDir.cd(cachePath);
    }
    QFile *cookieFile = new QFile(configDir.absoluteFilePath("Cookies.dat"));//You have to change the path before release
    if(!cookieFile->open(QIODevice::WriteOnly)) qDebug()<<"Cookies.dat open for write failed";
    QDataStream cookieStream(cookieFile);
    QNetworkCookie cookie;
    int count = cookieList.count();
    cookieStream << count;
    for(int i = 0; i< count; i++) {
        cookie = allCookies().at(i);
        if(!cookie.isSessionCookie()) cookieStream<<cookie.toRawForm();
    }
    cookieFile->close();
}

void CookieJar::load() {
    QString cachePath = m_configPath + "/cache";

    QList<QNetworkCookie> restoredCookies;
    QDir configDir = QDir::home();
    if(!configDir.cd(cachePath)) {
        qDebug()<<"Making path "<<QDir::cleanPath(cachePath);
        configDir.mkpath(QDir::cleanPath(cachePath));
        configDir.cd(cachePath);
    }
    QFile *cookieFile = new QFile(configDir.absoluteFilePath("Cookies.dat"));
    if(!cookieFile->open(QIODevice::ReadOnly)) {
        qDebug()<<"Cookies.dat open for read failed";
        return;
    }
    int count;
    QDataStream cookieStream(cookieFile);
    cookieStream>>count;
    for(int i = 0; i<count; i++) {
        QByteArray cookieData;
        cookieStream >> cookieData;
        const QList<QNetworkCookie> &cookieList = QNetworkCookie::parseCookies(cookieData);
        if(cookieList.isEmpty()) continue;
        const QNetworkCookie &cookie = cookieList.at(0);
        restoredCookies.append(cookie);
    }

    setAllCookies(restoredCookies);
    cookieFile->close();
}
