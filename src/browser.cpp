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

#include <QDir>
#include <QDesktopServices>
#include <QDebug>
#include "browser.h"

Browser::Browser(QWidget *parent, QString configPath) : QWebView(parent)
{
    m_configPath = configPath;
    setupBrowser();

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    m_tableMessageTypes.insert("composerInput.input._52t1", CHAT);
    m_tableMessageTypes.insert("composerInput.composerInput.mentions-input", COMMENT);

    for (int i = 1; i <= 10; i++)
        m_tableMessageTypes.insert(QString("uniqid_%1.composerInput.mentions-input").arg(QString::number(i)), STATUS);

    connect(this, SIGNAL(linkClicked(QUrl)),this,SLOT(openExternalLink(QUrl)));
    connect(page()->networkAccessManager(), SIGNAL( sslErrors( QNetworkReply*, const QList<QSslError> & ) ), this, SLOT( sslErrorHandler( QNetworkReply*, const QList<QSslError> & ) ) );

    load(QUrl("http://m.facebook.com"));

    this->setEnabled(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTextEditor()));
    timer->start(100);
}

Browser::~Browser()
{
    cookieJar->save();
    //delete cookieJar;
}

void Browser::setupBrowser()
{
    QString cachePath = m_configPath + "/cache";

    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
    QDir configDir = cachePath;
    configDir.mkpath(cachePath);
    configDir.mkpath(cachePath + "/LocalStorage/");
    configDir.mkpath(cachePath + "/OfflineStorage/");
    configDir.mkpath(cachePath + "/IconDatabase/");
    configDir.mkpath(cachePath + "/OfflineWebApplicationCache/");
    configDir.cd(cachePath);
    configDir.cd("OfflineStorage");
    QWebSettings::setOfflineStoragePath(configDir.absolutePath());
    configDir.cdUp();
    configDir.cd("IconDatabase");
    QWebSettings::setIconDatabasePath(configDir.absolutePath());
    configDir.cdUp();
    configDir.cd("OfflineWebApplicationCache");
    QWebSettings::setOfflineWebApplicationCachePath(configDir.absolutePath());
    configDir.cdUp();
    QWebSettings::setMaximumPagesInCache(100);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
    cookieJar = new CookieJar(this, cachePath);
    page()->networkAccessManager()->setCookieJar(cookieJar);
    cookieJar->load();
    configDir.cd("LocalStorage");
    page()->settings()->setLocalStoragePath(configDir.absolutePath());
    page()->settings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
    configDir.cdUp();
}

/*
 * Used by timer. When a TEXTEDIT is focused this function
 * sends a signal to show the text editor
*/
void Browser::showTextEditor()
{
    if (isEditableTaxtareaFocused() /* TODO: && fare una funzione che controlla se sono in zona di text aree note*/) {
        hideSendButtons();  // retrieveSendButtons?

        if (!m_textEditor_flag)
            emit textareaFocused();

        m_textEditor_flag = true;
    }
    else {
        if (!m_textEditor_flag) {
            m_textArea.removeAttribute("style");
            sendBtn.removeAttribute("style");

            emit hideTextarea();
        }

        m_textEditor_flag = false;
    }
}

/*
 * Retrieves the current TEXTAREA element if focused
*/

bool Browser::isEditableTaxtareaFocused()
{
    /*
        TEXTAREAS UIDs

        Messages: composerInput.input._52t1
        Comments: composerInput.composerInput.mentions-input
        Statuses: uniqid_1.composerInput.mentions-input
    */

    QWebFrame *frame = page()->currentFrame();

    if (frame!=NULL)
    {
        QWebElement document = frame->documentElement();
        QWebElementCollection collection = document.findAll("TEXTAREA");

        foreach (QWebElement paraElement, collection) {
            if (paraElement.hasFocus()) {
                QString textareaUID = paraElement.attribute("id") + "." + paraElement.attribute("class").replace(" ", ".");
                //qDebug() << "id+class: " << textareaUID;
                //qDebug() << "xml: " << paraElement.toOuterXml();
                //paraElement.setAttribute("style", "display:none;");

                if (m_tableMessageTypes.contains(textareaUID)) {
                    m_messageType = m_tableMessageTypes[textareaUID];

                    m_textArea = paraElement;

                    return true;
                }
            }
        }
    }
    return false;
}

void Browser::hideSendButtons()
{
    getSendButton();
}

QWebElement Browser::getSendButton()
{
    /*
        Inner html of buttons of interest:

        Chat reply: <button type="submit" value="Reply" class="_52cp btn btnC mfss touchable" name="send" role="button" data-sigil="m-messaging-button blocking-touchable">Reply</button>
        Chat like: <button type="submit" value="Reply" class="_55uw btn btnC mfss touchable iconOnly" name="like" role="button" aria-label="Reply" data-sigil="blocking-touchable"><i class="img sp_bj32xz sx_bfa071" style="margin-top: 4px;"></i></button>
        Commento: <button type="submit" value="Comment" class="touchable _56bs _5of- _56bu" name="post" data-sigil="touchable composer-submit"><span class="_55sr">Comment</span></button>
        Post status: <button type="submit" value="Post" class="btn btnI bgb mfss touchable" data-store="{&quot;nativeClick&quot;:true}" role="button" data-sigil="submit_composer touchable">Post</button>
    */

    switch (m_messageType) {
    case CHAT:      // Reply button
        sendBtn = findButtonByValue("submit", "send");
        break;
    case COMMENT:   // Comment button
        sendBtn = findButtonByValue("submit", "post");
        break;
    case STATUS:    // Post button
        sendBtn = findButtonByValue("submit", "");
        break;
    }

    //sendBtn.setAttribute("style", "display:none;");

    return sendBtn;
}

QWebElement Browser::findButtonByValue(QString type, QString name)
{
    QWebFrame *frame = page()->currentFrame();
    QWebElement e;

    if (frame != NULL)
    {
        QWebElement document = frame->documentElement();
        QWebElementCollection collection = document.findAll("button");

        foreach (QWebElement paraElement, collection) {
            if ((paraElement.attribute("type") == type) && (paraElement.attribute("name") == name)) {

                e = paraElement;
            }
        }
    }

    return e;
}

/*
 * Set the text into the focused TEXTAREA
 * and simulate the mouseclick on the send/reply button
*/
void Browser::setTextAreaContent(QString text)
{
    // Set the text to sent

    /*if (m_messageType == STATUS) {
        QWebFrame *frame = page()->currentFrame();

        if (frame != NULL)
        {
            QWebElement document = frame->documentElement();
            QWebElementCollection collection = document.findAll("input");

            foreach (QWebElement paraElement, collection) {
                //qDebug() << "Elemento in lista: " << paraElement.toOuterXml();
                if (paraElement.attribute("name") == "message") {
                    paraElement.setAttribute("value", text);

                    qDebug() << "Elemento status: " << paraElement.attribute("name");
                    qDebug() << "Elemento in lista: " << paraElement.toOuterXml();

                    break;
                }
            }
        }
    }
    else
        m_textArea.setPlainText(text);*/

    m_textArea.evaluateJavaScript("this.value = '" + text + "';");
    sendBtn.evaluateJavaScript("this.click();");
    //m_textArea.evaluateJavaScript("var evObj = document.createEvent('KeyboardEvent');evObj.initEvent('keypress', true, true, window, 0, 0, 0, 0, 0, 'e'.charCodeAt(0));this.dispatchEvent(evObj);");
    //m_textArea.evaluateJavaScript("var e = document.createEvent('KeyboardEvent');e.initKeyboardEvent('keydown', true, true, document.defaultView, 'Enter', 0, '', false, '');this.dispatchEvent(e);");

    clickSendButtonStatus();
    QTimer::singleShot(2000, this, SLOT(clickSendButtonStatus()));

    // Click the the send button
    //sendBtn.evaluateJavaScript("var evObj = document.createEvent('MouseEvents');evObj.initEvent( 'click', true, true );this.dispatchEvent(evObj);");

    emit clearTextEditChat();

    //m_textArea.setPlainText("");

    // Re-click on comments (fixes a sort of bug)
    //if (m_messageType == COMMENT)
        //QTimer::singleShot(2000, this, SLOT(clickSendButton()));
}

void Browser::gotoPage(Browser::FbPage page)
{
    QString sUrl;

    switch (page) {
    case(HOME):
        sUrl = "https://m.facebook.com/";
        break;
    case(MESSAGES):
        sUrl = "https://m.facebook.com/messages";
        break;
    case(NOTIFICATIONS):
        sUrl = "https://m.facebook.com/notifications";
        break;
    }

    load(QUrl(sUrl));
}

void Browser::sslErrorHandler(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->ignoreSslErrors( errors );
}

void Browser::clickSendButton()
{
    m_textArea.setPlainText("");

    sendBtn.evaluateJavaScript("var evObj = document.createEvent('MouseEvents');evObj.initEvent( 'click', true, true );this.dispatchEvent(evObj);");
}

void Browser::clickSendButtonStatus()
{
    qDebug() << "Button: " << sendBtn.toOuterXml();
    sendBtn.evaluateJavaScript("var evObj = document.createEvent('MouseEvents');evObj.initEvent( 'click', true, true );this.dispatchEvent(evObj);");
}

void Browser::openExternalLink(QUrl url) {
    if(url.toString().contains("facebook.com/l.php?u=")) {
        QDesktopServices::openUrl(url);
    } else {
        load(url);
    }
}
