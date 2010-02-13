QT += webkit \
    network \
    xml
unix { 
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib \
        }
    win32 { 
        INCLUDEPATH += "C:\Program Files\qjson\include"
        LIBS += -L"C:\Program Files\qjson\bin"
    }
    LIBS += -lqjson
    SOURCES += main.cpp \
        fbconnectwizard.cpp \
        webview.cpp \
        qtfacebook.cpp \
        userinfo.cpp \
        testqueryconsole.cpp \
        api/apimethod.cpp \
        api/factory.cpp \
        api/comments_get.cpp \
        api/friends_get.cpp \
        api/notifications_get.cpp \
        api/notifications_getlist.cpp \
        api/users_getloggedinuser.cpp \
        api/notifications_markread.cpp \
        gui/notifications_listview.cpp \
        gui/notifications_widget.cpp \
        data/notification.cpp \
        data/appinfo.cpp \
        gui/notificationlabel.cpp \
        gui/appinfolabel.cpp \
        gui/notificationcenter.cpp \
        api/fql_GetNewNotifications.cpp \
        api/fql_GetAppInfo.cpp \
        gui/notificationcenterlabel.cpp \
        gui/notificationcenterwidget.cpp \
        util/facebooklogin.cpp \
        util/cookiejar.cpp \
        gui/facebooklogindialog.cpp \
        gui/loginfaileddialog.cpp \
        data/chatmessage.cpp \
        data/buddy.cpp \
        cometconnection.cpp \
        api/fql_GetStreamPosts.cpp
    HEADERS += fbconnectwizard.h \
        webview.h \
        qtfacebook.h \
        userinfo.h \
        testqueryconsole.h \
        ui_testqueryconsole.h \
        api/factory.h \
        api/comments_get.h \
        api/friends_get.h \
        api/apimethod.h \
        api/notifications_get.h \
        api/notifications_getlist.h \
        api/users_getloggedinuser.h \
        api/notifications_markread.h \
        gui/notifications_listview.h \
        gui/notifications_widget.h \
        data/notification.h \
        data/appinfo.h \
        gui/notificationlabel.h \
        gui/appinfolabel.h \
        gui/notificationcenter.h \
        api/fql_GetNewNotifications.h \
        api/fql_GetAppInfo.h \
        gui/notificationcenterlabel.h \
        gui/notificationcenterwidget.h \
        util/facebooklogin.h \
        util/cookiejar.h \
        gui/facebooklogindialog.h \
        gui/loginfaileddialog.h \
        data/chatmessage.h \
        data/buddy.h \
        cometconnection.h \
        api/fql_GetStreamPosts.h
    OTHER_FILES += uiImages/thumbsUp3.jpg \
        uiImages/thumbsDown3.jpg \
        uiImages/signGuy.jpg \
        uiImages/facebook-small-logo-thumb-360x360-75537-thumb-300x300-78195.png \
        Doxyfile
    FORMS += testqueryconsole.ui
    RESOURCES += resources.qrc
    HEADERS += util/workerthread.h \
        util/worker.h
    SOURCES += util/workerthread.cpp \
        util/worker.cpp
    HEADERS += updatepoller.h
    SOURCES += updatepoller.cpp
HEADERS += data/streampost.h \
    data/streamcommentlist.h
SOURCES += data/streampost.cpp \
    data/streamcommentlist.cpp
