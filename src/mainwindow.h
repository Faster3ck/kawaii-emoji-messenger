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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QResizeEvent>
#include <QProgressBar>
#include <QSystemTrayIcon>
#include "listwidgetemoji.h"
#include "browser.h"
#include "comboemojicategory.h"
#include "frameemoji.h"
#include "trayicon.h"
#include "updatechecker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    Browser *browser;
    QProgressBar *progressBar;
    QString m_configPath;
    EmojiManager *emojiMan;
    FrameEmoji *frameEmoji;
    QListWidget *m_listWidget;
    TrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QString m_emoji_location;

    void loadComboCategories();

    QString htmlChatToText();

    void showHideWindow();

private slots:
    void loadStarted();
    void loadBrowserProgress(int progress);
    void loadBrowserFinished(bool ok);
    void showEmojiPopup();
    void insertEmoticon(QString symbol);
    void on_pushSend_clicked();
    void openTextEditor();
    void hideTextEditor();
    void on_actionHome_triggered();
    void on_actionMessages_triggered();
    void on_actionNotifications_triggered();
    void clearTextEditChat();           // When the message is sent
    void on_pushEmojiView_clicked();
    void copyCurrentUrl();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_actionExit_triggered();
    void on_actionShowHideWindow_triggered();
    void checkForUpdates();
    void on_actionToolsSettings_triggered();
    void on_actionHelpAbout_triggered();
    void on_actionHelpDonate_triggered();
};

#endif // MAINWINDOW_H
