QT += webkit \
    network \
    xml
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
    api/fql_GetStreamPosts.cpp \
    util/workerthread.cpp \
    util/worker.cpp \
    updatepoller.cpp \
    data/streampost.cpp \
    data/streamcommentlist.cpp \
    gui/streampostwidget.cpp \
    data/fbuserinfo.cpp \
    gui/commentwidget.cpp \
    api/fql_getstreampostinfo.cpp \
    util/fbuserpiccache.cpp \
    data/FbPageInfo.cpp
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
    api/fql_GetStreamPosts.h \
    data/fbuserinfo.h \
    gui/streampostwidget.h \
    data/streampost.h \
    data/streamcommentlist.h \
    updatepoller.h \
    util/workerthread.h \
    util/worker.h \
    util/agestring.h \
    gui/commentwidget.h \
    api/fql_getstreampostinfo.h \
    gui/notificationcenteritem.h \
    util/fbuserpiccache.h \
    data/FbPageInfo.h
OTHER_FILES += uiImages/thumbsUp3.jpg \
    uiImages/thumbsDown3.jpg \
    uiImages/signGuy.jpg \
    uiImages/facebook-small-logo-thumb-360x360-75537-thumb-300x300-78195.png \
    Doxyfile
FORMS += testqueryconsole.ui
RESOURCES += resources.qrc
