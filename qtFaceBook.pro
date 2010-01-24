QT += webkit \
    network
SOURCES += main.cpp \
    fbconnectwizard.cpp \
    webview.cpp \
    cookiejar.cpp \
    qtfacebook.cpp \
    userinfo.cpp \
    testqueryconsole.cpp \
    api/apimethod.cpp
HEADERS += fbconnectwizard.h \
    webview.h \
    cookiejar.h \
    qtfacebook.h \
    userinfo.h \
    testqueryconsole.h \
    ui_testqueryconsole.h \
    api/apimethod.h \
    api/observer.h
OTHER_FILES += uiImages/thumbsUp3.jpg \
    uiImages/thumbsDown3.jpg \
    uiImages/signGuy.jpg
FORMS += testqueryconsole.ui
RESOURCES += resources.qrc
