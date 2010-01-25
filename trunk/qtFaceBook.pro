QT += webkit \
    network
SOURCES += main.cpp \
    fbconnectwizard.cpp \
    webview.cpp \
    cookiejar.cpp \
    qtfacebook.cpp \
    userinfo.cpp \
    testqueryconsole.cpp \
    api/apimethod.cpp \
    api/factory.cpp \
    api/comments_get.cpp \
    api/friends_get.cpp
HEADERS += fbconnectwizard.h \
    webview.h \
    cookiejar.h \
    qtfacebook.h \
    userinfo.h \
    testqueryconsole.h \
    ui_testqueryconsole.h \
    api/apimethod.h \
    api/observer.h \
    api/factory.h \
    api/comments_get.h \
    api/friends_get.h
OTHER_FILES += uiImages/thumbsUp3.jpg \
    uiImages/thumbsDown3.jpg \
    uiImages/signGuy.jpg
FORMS += testqueryconsole.ui
RESOURCES += resources.qrc
