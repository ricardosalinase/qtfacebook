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
    api/friends_get.cpp \
    api/notifications_get.cpp \
    api/notifications_getlist.cpp
HEADERS += fbconnectwizard.h \
    webview.h \
    cookiejar.h \
    qtfacebook.h \
    userinfo.h \
    testqueryconsole.h \
    ui_testqueryconsole.h \
    api/factory.h \
    api/comments_get.h \
    api/friends_get.h \
    api/apimethod.h \
    api/notifications_get.h \
    api/notifications_getlist.h
OTHER_FILES += uiImages/thumbsUp3.jpg \
    uiImages/thumbsDown3.jpg \
    uiImages/signGuy.jpg \
    uiImages/facebook-small-logo-thumb-360x360-75537-thumb-300x300-78195.png
FORMS += testqueryconsole.ui
RESOURCES += resources.qrc
