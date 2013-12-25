#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T17:39:46
#
#-------------------------------------------------

QT       += core gui webkit network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kawaii-emoji-messenger
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/listwidgetemoji.cpp \
    src/browser.cpp \
    src/cookiejar.cpp \
    src/emojimanager.cpp \
    src/comboemojicategory.cpp \
    src/frameemoji.cpp \
    src/trayicon.cpp \
    src/updatechecker.cpp \
    src/dialogsetting.cpp \
    src/whereiam.cpp \
    src/dialogabout.cpp

HEADERS  += src/mainwindow.h \
    src/listwidgetemoji.h \
    src/browser.h \
    src/cookiejar.h \
    src/emojimanager.h \
    src/comboemojicategory.h \
    src/frameemoji.h \
    src/trayicon.h \
    src/updatechecker.h \
    src/dialogsetting.h \
    src/globals.h \
    src/whereiam.h \
    src/dialogabout.h

FORMS    += ui/mainwindow.ui \
    ui/frameemoji.ui \
    ui/dialogsetting.ui \
    ui/dialogabout.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
