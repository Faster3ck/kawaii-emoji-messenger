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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QSizePolicy>
#include <QDesktopServices>

#include "dialogsetting.h"
#include "dialogabout.h"
#include "globals.h"
#include "whereiam.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(globals::PROGRAM_NAME);

    m_configPath = QString("%1%2")
            .arg(QDesktopServices::storageLocation(QDesktopServices::DataLocation))
            .arg(globals::PROGRAM_NAME);

    browser = new Browser(this, m_configPath);
    setCentralWidget(browser);
    browser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);

    emojiMan = new EmojiManager();
    frameEmoji = new FrameEmoji();
    trayIcon = new TrayIcon(this);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(ui->actionShowHideWindow);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui->actionExit);

    trayIcon->setContextMenu(trayIconMenu);

    /* Progressbar for browser! */
    progressBar = new QProgressBar(this);
    ui->statusBar->addPermanentWidget(progressBar, 0);
    progressBar->setMaximumSize(QSize(200, 500));

    connect(frameEmoji, SIGNAL(sendEmoji(QString)), this, SLOT(insertEmoticon(QString)));

    connect(browser, SIGNAL(loadProgress(int)), this, SLOT(loadBrowserProgress(int)));
    connect(browser, SIGNAL(loadFinished(bool)), this, SLOT(loadBrowserFinished(bool)));
    connect(browser, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
    connect(browser, SIGNAL(textareaFocused()), this, SLOT(openTextEditor()));
    connect(browser, SIGNAL(hideTextarea()), this, SLOT(hideTextEditor()));
    connect(browser, SIGNAL(clearTextEditChat()), this, SLOT(clearTextEditChat()));

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionNavBack, SIGNAL(triggered()), browser, SLOT(back()));
    connect(ui->actionNavNext, SIGNAL(triggered()), browser, SLOT(forward()));
    connect(ui->actionNavReload, SIGNAL(triggered()), browser, SLOT(reload()));
    connect(ui->actionNavCopyCurrentUrl, SIGNAL(triggered()), this, SLOT(copyCurrentUrl()));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    ui->dockWidget->setVisible(false);

    QTimer::singleShot(10000, this, SLOT(checkForUpdates()));
}

MainWindow::~MainWindow()
{
    delete browser;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    showHideWindow();
    event->ignore();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        showHideWindow();
        break;
    case QSystemTrayIcon::Unknown:
        break;
    case QSystemTrayIcon::Context:
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

/*
 * Convert html textEdit to plainText
 * Used when user clicks on sendButton
*/
QString MainWindow::htmlChatToText()
{
    QRegExp rx("<a [^>]*name=\"([^\"]+)\"[^>]*>");

    QString str = ui->textEditChat->toHtml();

    QStringList list;
    list.clear();
    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    for (int i = 0; i < list.count(); i++) {
        QString emojiNumber = list.at(i);

        QString searchTag = QString("<a name=\"%1\"></a>")
                .arg(emojiNumber);

        QString replTag = emojiMan->getEmojiSymbolFromNumber(emojiNumber);

        str = str.replace(searchTag, replTag);
    }

    QTextDocument doc;
    doc.setHtml(str);

    return doc.toPlainText();
}

void MainWindow::showHideWindow()
{
    if (this->isVisible()) {
        hide();
        ui->actionShowHideWindow->setText(tr("Restore"));
    }
    else {
        show();
        ui->actionShowHideWindow->setText(tr("Hide"));
    }
}

void MainWindow::loadStarted()
{
    progressBar->setVisible(true);
}

void MainWindow::loadBrowserProgress(int progress)
{
    progressBar->setValue(progress);
}

void MainWindow::loadBrowserFinished(bool ok)
{
    if (ok)
        progressBar->hide();

    ui->statusBar->showMessage(tr("Page loaded"), 2000);
}

void MainWindow::showEmojiPopup()
{
    QPoint p = QCursor::pos();
    frameEmoji->setGeometry(p.x(), p.y() - 100, 300, 250);
    frameEmoji->show();
}

/* Inserts emoji when user double-click on emoji grid.
 * This function converts plain text into html
 *
 * Used by frameEmoji->sendEmoji signal
 */
void MainWindow::insertEmoticon(QString symbol)
{
    QTextCursor cursor(ui->textEditChat->textCursor());

    if (!cursor.isNull()) {
        cursor.insertHtml(symbol);
    }

    QTextCursor cursor2(ui->textEditChat->document()->find(symbol));

    QString emojiNumber = emojiMan->getEmojiNumberFromSymbol(symbol);

#ifdef Q_OS_LINUX
    QDir iconsLocation(WhereIAm::dataDir() + "/kawaii-emoji-messenger/data/icons");
#else
    QDir iconsLocation(WhereIAm::dataDir() + "/data/icons");
#endif

    QString s = QString("%1/x-%2.png")
            .arg(iconsLocation.absolutePath())
            .arg(emojiNumber);

    if (!cursor2.isNull()) {
        QString imgTag = QString("<img src=\"%1\" id=\"%2\" />")
                .arg(s)
                .arg(emojiNumber);

        cursor2.insertHtml(imgTag);
    }
}

// Send button
void MainWindow::on_pushSend_clicked()
{
    QString text = htmlChatToText();

    browser->setTextAreaContent(text);
}

void MainWindow::openTextEditor()
{
    ui->textEditChat->clear();
    ui->dockWidget->setVisible(true);

    ui->textEditChat->setFocus();
}

void MainWindow::hideTextEditor()
{
    ui->dockWidget->close();
}

void MainWindow::on_actionHome_triggered()
{
    browser->gotoPage(Browser::HOME);
}

void MainWindow::on_actionMessages_triggered()
{
    browser->gotoPage(Browser::MESSAGES);
}

void MainWindow::on_actionNotifications_triggered()
{
    browser->gotoPage(Browser::NOTIFICATIONS);
}

// When the message is sent
void MainWindow::clearTextEditChat()
{
    ui->textEditChat->setText("");
    ui->textEditChat->document()->setHtml("");
}

void MainWindow::on_pushEmojiView_clicked()
{
    showEmojiPopup();
}

void MainWindow::copyCurrentUrl()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(browser->url().toString());

    ui->statusBar->showMessage(tr("Url copied"), 1000);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionShowHideWindow_triggered()
{
    showHideWindow();
}

void MainWindow::checkForUpdates()
{
    UpdateChecker *up = new UpdateChecker();
    up->checkForUpdates();
}

void MainWindow::on_actionToolsSettings_triggered()
{
    DialogSetting dlg(this, m_configPath);
    dlg.exec();
}

void MainWindow::on_actionHelpAbout_triggered()
{
    DialogAbout dlg;
    dlg.exec();
}

void MainWindow::on_actionHelpDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=BPS56LV66S6JS"));
}

void MainWindow::on_actionBugReport_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Faster3ck/kawaii-emoji-messenger/issues"));
}
