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

#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QObject>
#include <QWebView>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include <QTimer>
#include <QHash>
#include <QNetworkReply>
#include "cookiejar.h"

class Browser : public QWebView
{
    Q_OBJECT

public:
    enum FbPage {
        HOME,
        MESSAGES,
        NOTIFICATIONS
    };

    explicit Browser(QWidget *parent = 0, QString configPath = "");
    ~Browser();

    void setTextAreaContent(QString text);
    void gotoPage(FbPage page);

private:
    /* Types of messages */
    enum FbMessageType {
        CHAT,
        COMMENT,
        STATUS
    };

    void setupBrowser();
    bool isEditableTaxtareaFocused();
    void hideSendButtons();
    QWebElement getSendButton();
    QWebElement findButtonByValue(QString type, QString name);

    CookieJar *cookieJar;
    QTimer *timer;
    QString m_configPath;

    QWebElement sendBtn;
    QWebElement m_textArea;

    FbMessageType m_messageType;    // What kind of message I'm sending (chat, comment or status)
    QHash<QString, FbMessageType> m_tableMessageTypes;

    bool m_textEditor_flag;         // if text editor is visible

signals:
    void textareaFocused();
    void hideTextarea();
    void clearTextEditChat();
    
private slots:
    void showTextEditor();
    void sslErrorHandler( QNetworkReply *reply, const QList<QSslError> &errors );
    void clickSendButton();
    void clickSendButtonStatus();
    void openExternalLink(QUrl url);
};

#endif // BROWSER_H
