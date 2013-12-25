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

#include <QMessageBox>
#include <QDir>
#include "dialogsetting.h"
#include "ui_dialogsetting.h"

DialogSetting::DialogSetting(QWidget *parent, QString configPath) :
    QDialog(parent),
    ui(new Ui::DialogSetting)
{
    ui->setupUi(this);
    m_configPath = configPath;

    QDir settingDir(configPath);

    ui->lineEditStorage->setText(settingDir.absolutePath());
}

DialogSetting::~DialogSetting()
{
    delete ui;
}

void DialogSetting::clearCache()
{
    QString cachePath = m_configPath + "/cache";
    QDir cacheDir(cachePath);

    int ret = QMessageBox::warning(0, tr("Delete cache?"),
                                   tr("Would you clean cache and cookies?"),
                                   QMessageBox::Ok | QMessageBox::No);

    if (ret == QMessageBox::Ok) {
        if (removeDir(cacheDir.absolutePath()))
            QMessageBox::information(0, tr("Done!"),
                                               tr("Cache and cookies has been removed!"),
                                               QMessageBox::Ok);
        else
            QMessageBox::critical(0, tr("Error!"),
                                               tr("Error in removing cache and coockies!"),
                                               QMessageBox::Ok);
    }
}

void DialogSetting::on_pushClose_clicked()
{
    close();
}

bool DialogSetting::removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

void DialogSetting::on_pushClearCache_clicked()
{
    clearCache();
}
