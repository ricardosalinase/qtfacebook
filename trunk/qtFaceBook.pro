message($$CONFIG)
defineTest(hasAspell) { 
    libdirs = /usr/local/lib \
        /usr/lib
    for(libdir, libdirs):exists($${libdir}/libaspell.so):return(true)
    return(false)
}
QT += webkit \
    network \
    xml
hasAspell() { 
    SOURCES += SpellTextEdit/SpellTextEdit.cpp \
        SpellTextEdit/SpellSyntaxHighlighter.cpp \
        SpellTextEdit/ASpellConfig.cpp
    HEADERS += SpellTextEdit/SpellTextEdit.h \
        SpellTextEdit/SpellSyntaxHighlighter.h \
        SpellTextEdit/ASpellConfig.h
    LIBS += -laspell
    DEFINES += WITH_ASPELL
}
SOURCES += main.cpp \
    fbconnectwizard.cpp \
    webview.cpp \
    qtfacebook.cpp \
    testqueryconsole.cpp \
    api/apimethod.cpp \
    api/factory.cpp \
    api/friends_get.cpp \
    api/notifications_getlist.cpp \
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
    util/cookiejar.cpp \
    api/fql_GetStreamPosts.cpp \
    util/workerthread.cpp \
    util/worker.cpp \
    updatepoller.cpp \
    data/streampost.cpp \
    data/streamcommentlist.cpp \
    gui/streampostwidget.cpp \
    data/fbuserinfo.cpp \
    gui/commentwidget.cpp \
    util/fbuserpiccache.cpp \
    data/FbPageInfo.cpp \
    data/FbPhoto.cpp \
    data/FbStreamAttachment.cpp \
    data/FbStreamMedia.cpp \
    data/FbStreamAttachmentProperty.cpp \
    api/fql_GetComments.cpp \
    api/stream_RemoveComment.cpp \
    api/stream_AddComment.cpp \
    api/notifications_MarkRead.cpp \
    util/FbPhotoCache.cpp \
    gui/FbPhotoLabel.cpp \
    gui/FbPhotoViewWidget.cpp \
    gui/FbCommentManager.cpp \
    gui/FbAttachmentWidget.cpp \
    api/fql_GetPhotos.cpp \
    data/FbAlbum.cpp \
    api/comments_Add.cpp \
    api/comments_Remove.cpp \
    gui/LoadingProgressWidget.cpp \
    util/OurUserInfo.cpp \
    api/fql_GetAlbums.cpp \
    gui/FbAlbumViewWidget.cpp \
    gui/ImageLabel.cpp \
    util/HyperLink.cpp \
    gui/FbLikeManager.cpp \
    api/fql_GetLikes.cpp \
    api/stream_AddLike.cpp \
    api/stream_RemoveLike.cpp \
    api/users_GetLoggedInUser.cpp

# gui/BuddyListWidget.cpp \
# FbChatConnection.cpp \
# data/FbBuddy.cpp \
# gui/BuddyWidget.cpp
HEADERS += fbconnectwizard.h \
    webview.h \
    qtfacebook.h \
    testqueryconsole.h \
    ui_testqueryconsole.h \
    api/factory.h \
    api/friends_get.h \
    api/apimethod.h \
    api/notifications_getlist.h \
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
    util/cookiejar.h \
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
    gui/notificationcenteritem.h \
    util/fbuserpiccache.h \
    data/FbPageInfo.h \
    data/FbPhoto.h \
    data/FbStreamAttachment.h \
    data/FbStreamMedia.h \
    data/FbStreamAttachmentProperty.h \
    api/fql_GetComments.h \
    api/stream_RemoveComment.h \
    api/stream_AddComment.h \
    api/notifications_MarkRead.h \
    util/FbPhotoCache.h \
    gui/FbPhotoLabel.h \
    gui/FbPhotoViewWidget.h \
    gui/FbCommentManager.h \
    gui/FbAttachmentWidget.h \
    api/fql_GetPhotos.h \
    data/FbAlbum.h \
    api/comments_Add.h \
    api/comments_Remove.h \
    gui/LoadingProgressWidget.h \
    util/OurUserInfo.h \
    api/fql_GetAlbums.h \
    gui/FbAlbumViewWidget.h \
    gui/ImageLabel.h \
    util/HyperLink.h \
    gui/FbLikeManager.h \
    api/fql_GetLikes.h \
    api/stream_AddLike.h \
    api/stream_RemoveLike.h \
    api/users_GetLoggedInUser.h

# gui/BuddyListWidget.h \
# FbChatConnection.h \
# data/FbBuddy.h \
# gui/BuddyWidget.h
OTHER_FILES += uiImages/thumbsUp3.jpg \
    uiImages/thumbsDown3.jpg \
    uiImages/signGuy.jpg \
    uiImages/facebook-small-logo-thumb-360x360-75537-thumb-300x300-78195.png \
    Doxyfile \
    uiImages/thumbUp_50_50.jpg \
    uiImages/thumbDown_50_50.jpg \
    uiImages/addComment_50_50.jpg
FORMS += testqueryconsole.ui
RESOURCES += resources.qrc
