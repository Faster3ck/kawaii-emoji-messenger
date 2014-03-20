PROJECT(kawaii-emoji-messenger)

cmake_minimum_required(VERSION 2.4)
  
SET(GUI "kawaii-emoji-messenger")  
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTWEBKIT TRUE)

ADD_DEFINITIONS( ${OPTIMIZATION_FLAGS} )
  
SET(kawaii-emoji-messenger_UIS  
    ui/mainwindow.ui
    ui/dialogsetting.ui
    ui/frameemoji.ui
    ui/emojipopup.ui
    ui/dialogabout.ui
)  
  
SET(kawaii-emoji-messenger_SRCS  
    src/browser.cpp
    src/comboemojicategory.cpp
    src/cookiejar.cpp
    src/dialogabout.cpp
    src/dialogsetting.cpp
    src/emojimanager.cpp
    src/frameemoji.cpp
    src/listwidgetemoji.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/trayicon.cpp
    src/updatechecker.cpp
    src/whereiam.cpp
)  
  
SET(kawaii-emoji-messenger_MOC_HDRS  
    src/browser.h
    src/comboemojicategory.h
    src/cookiejar.h
    src/dialogabout.h
    src/dialogsetting.h
    src/emojimanager.h
    src/frameemoji.h
    src/globals.h
    src/listwidgetemoji.h
    src/mainwindow.h
    src/trayicon.h
    src/updatechecker.h
    src/whereiam.h
)
  
SET(kawaii-emoji-messenger_RESOURCES  
    res.qrc
)

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(Qt4 COMPONENTS QtWebKit REQUIRED)
FIND_PACKAGE(Qt4 COMPONENTS QtNetwork REQUIRED)
find_package(Qt4 REQUIRED QtCore QtGui)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_UI(kawaii-emoji-messenger_UIS_H ${kawaii-emoji-messenger_UIS})  
QT4_WRAP_CPP(kawaii-emoji-messenger_MOC_SRCS ${kawaii-emoji-messenger_MOC_HDRS})  
QT4_ADD_RESOURCES(kawaii-emoji-messenger_RESOURCES ${kawaii-emoji-messenger_RESOURCES})

ADD_DEFINITIONS(
    -Wall
    -W
    ${QT_DEFINITIONS}
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDES} . )
  
ADD_EXECUTABLE(${GUI}  
    ${kawaii-emoji-messenger_SRCS}  
    ${kawaii-emoji-messenger_UIS_H}  
    ${kawaii-emoji-messenger_MOC_SRCS}  
    ${kawaii-emoji-messenger_RESOURCES}  
    ${kawaii-emoji-messenger_RCS} 
)  
  
TARGET_LINK_LIBRARIES(${GUI} ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/kawaii-emoji-messenger DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES res/kawaii-emoji-messenger.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES res/kawaii-emoji-messenger.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)
install(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kawaii-emoji-messenger/data FILES_MATCHING PATTERN "*")
