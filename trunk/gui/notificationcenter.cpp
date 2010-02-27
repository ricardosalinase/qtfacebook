#include <QSettings>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QNetworkReply>
#include <QApplication>
#include <QDesktopWidget>
#include <QObject>
#include <QRegExp>

#include "api/factory.h"
#include "notificationcenter.h"
#include "appinfolabel.h"
#include "notificationlabel.h"
#include "notificationcenterwidget.h"
#include "streampostwidget.h"


namespace GUI {

NotificationCenter::NotificationCenter(UserInfo *userInfo, QWidget *parent) :
    QWidget(parent),
    m_userInfo(userInfo),
    m_showHiddenNotifications(false),
    m_showHiddenStreamPosts(true)
{
    m_scrollArea = new QScrollArea();
    m_scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 10px; }");
    m_scrollArea->setWidgetResizable(true);
    m_nContainer = new QWidget();
    m_nContainer->resize(200,600);
    m_nContainer->setStyleSheet("background: white;");
    QVBoxLayout *vbl = new QVBoxLayout();
    m_nContainer->setLayout(vbl);
    m_scrollArea->setWidget(m_nContainer);
    m_scrollArea->setStyleSheet("background: #e5e5e5");
    m_scrollArea->setAutoFillBackground(true);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(m_scrollArea);


    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    setStyleSheet("background: #526ea6");

    m_notificationList = new QList<DATA::Notification*>;

    m_factory = new API::Factory(m_userInfo);
    connect(m_factory, SIGNAL(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)),
            this, SLOT(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetNewNotificationsFailed(API::FQL::GetNewNotifications*)),
            this, SLOT(notificationGetFailed(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetStreamPosts(API::FQL::GetStreamPosts*)),
            this, SLOT(apiFqlGetStreamPosts(API::FQL::GetStreamPosts*)));
    connect(m_factory, SIGNAL(apiFqlGetStreamPostsFailed(API::FQL::GetStreamPosts*)),
            this, SLOT(getStreamPostsFailed(API::FQL::GetStreamPosts *)));
    connect(m_factory, SIGNAL(apiNotificationsMarkRead(API::Notifications::MarkRead*)),
            this,SLOT(notificationsMarkedAsRead(API::Notifications::MarkRead*)));
    connect(m_factory, SIGNAL(apiNotificationsMarkReadFailed(API::Notifications::MarkRead*)),
            this,SLOT(notificationsMarkedAsReadFailed(API::Notifications::MarkRead*)));

    m_nam = new QNetworkAccessManager();
    QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(receiveIconPixmap(QNetworkReply*)));

    getInitialNotifications();

}

void NotificationCenter::getInitialNotifications() {

    // I think we want to pre-fill the window so it's not empty
    // The downside is that it's only notifications with no feed
    // posts.
    API::Method *method = m_factory->createMethod("fql.multiquery.getNewNotifications");
    method->setArgument("start_time", 0);
    method->setArgument("only_read", 1); // don't get duplicate new ones
    method->setArgument("limit", 10);


    bool rc = method->execute();
    if (!rc)
        qDebug() << "Method error for fql.multiquery.getNewNotifications" << method->getErrorStr();

}

void NotificationCenter::notificationGetFailed(API::FQL::GetNewNotifications *method) {

    delete method;
    getInitialNotifications();


}

void NotificationCenter::apiFqlGetNewNotifications(API::FQL::GetNewNotifications *method) {

    m_notificationList = method->getNotificationList();

    qDebug() << "apiFqlGetNewNotifications(); m_notificationList: " << m_notificationList->size();

    newNotifications(m_notificationList);

    delete method;

}


void NotificationCenter::showYourself() {


    // The window flags thing is ... a hack. In theory you're
    // not supposed to be able to force a window to the front of the desktop.
    // This will.

    if (!isVisible()) {
        restoreWindow();
        show();
    } else {
        setWindowFlags(windowFlags() & Qt::WindowStaysOnTopHint);


        if (isMinimized())
            showNormal();
        else
            show();

        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }


}

void NotificationCenter::restoreWindow() {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    QByteArray g = settings.value("NotificationCenter").toByteArray();
    settings.endGroup();

    QDesktopWidget *dw = QApplication::desktop();
    QRect r = dw->screenGeometry();


    if (g.size() != 0)
    {
        restoreGeometry(g);
    }
    else
    {
        resize(200,600);
        move(r.width() - 240, 40);
    }

}


void NotificationCenter::apiFqlGetStreamPosts(API::FQL::GetStreamPosts *method) {

    QList<DATA::StreamPost *> *list = method->getStreamPosts();

    if (list->size())
    {
        DATA::StreamPost *sp = list->takeAt(0);
        m_streamPosts.insert(sp->getPostId(), sp);
        StreamPostWidget *spw = new StreamPostWidget(sp, m_userInfo);
        connect(spw, SIGNAL(closed(GUI::StreamPostWidget*)),
                this, SLOT(streamPostClosed(GUI::StreamPostWidget*)));
        spw->show();

    }

    delete list;
    method->deleteLater();

}


void NotificationCenter::getStreamPostsFailed(API::FQL::GetStreamPosts *method) {
    delete method;
}

void NotificationCenter::notificationAcknowledged(NotificationCenterItem::ItemType type, QString nId) {

    if (type == NotificationCenterItem::Notification)
    {
        // Mark the notification read via the API
        API::Method *method = m_factory->createMethod("notifications.markRead");

        method->setArgument("notification_ids",nId);
        method->execute();
    }
    else
    {
        emit acknowledgedNotification(type, nId);
    }


}

void NotificationCenter::notificationsMarkedAsRead(API::Notifications::MarkRead *method) {


    QString nId = method->getNotificationIds();
    emit acknowledgedNotification(NotificationCenterItem::Notification, nId);
    delete method;

}
void NotificationCenter::notificationsMarkedAsReadFailed(API::Notifications::MarkRead *method) {
    qDebug() << "notifications.markRead failed, resending.";
    QString nId = method->getNotificationIds();
    delete method;
    notificationAcknowledged(NotificationCenterItem::Notification, nId);
}

void NotificationCenter::newStreamPosts(QList<DATA::StreamPost *> *pList) {

    qDebug() << "NotificationCenter::newStreamPosts(); pList: " << pList->size();

    int numNew = 0;

    while (!pList->empty())
    {
        DATA::StreamPost *sp = pList->takeLast();

        GUI::NotificationCenterWidget *nWidget;
        QLabel *ul = new QLabel();

        if (sp->isFromUser())
            getPixmap(ul, sp->getPoster());
        else
            getPixmap(ul, sp->getPage());

        nWidget = new GUI::NotificationCenterWidget((GUI::NotificationCenterItem*)sp, ul);
        connect(nWidget, SIGNAL(linkActivated(QString)),
                this, SLOT(linkActivated(QString)));
        connect(nWidget, SIGNAL(acknowledged(NotificationCenterItem::ItemType,QString)),
                this, SLOT(notificationAcknowledged(NotificationCenterItem::ItemType, QString)));
        ((QVBoxLayout*)m_nContainer->layout())->insertWidget(0,nWidget);

        if (sp->isHidden() && !m_showHiddenStreamPosts)
            nWidget->hide();
        else
        {
            numNew++;
            nWidget->start();
        }

        m_streamPosts.insert(sp->getPostId(), sp);

    }

    if (numNew != 0)
        emit receivedNewStreamPosts(numNew);


}


void NotificationCenter::newNotifications(QList<DATA::Notification *> *nList) {

    qDebug() << "newNotifications(); nList: " << nList->size();
    while (!nList->empty())
    {
        DATA::Notification *n = nList->takeLast();
        newNotification(n);
    }
}

void NotificationCenter::newNotification(DATA::Notification *n) {

    int numNew = 0;

    if (!m_notifications.contains(n->getNotificationId()))
    {
        GUI::NotificationCenterWidget *nWidget;
        QLabel *al = new QLabel();

        getPixmap(al, n->getAppInfo());
        
        nWidget = new GUI::NotificationCenterWidget((GUI::NotificationCenterItem*)n, al);
        connect(nWidget, SIGNAL(linkActivated(QString)),
                this, SLOT(linkActivated(QString)));
        connect(nWidget, SIGNAL(acknowledged(NotificationCenterItem::ItemType,QString)),
                this, SLOT(notificationAcknowledged(NotificationCenterItem::ItemType, QString)));
        ((QVBoxLayout*)m_nContainer->layout())->insertWidget(0,nWidget);

        if (n->getIsHidden() && !m_showHiddenNotifications)
            nWidget->hide();
        else if (!n->getIsRead()){
            numNew++;
            m_notifications.insert(n->getNotificationId(), nWidget);
            nWidget->start();
        }

        if (numNew != 0)
            emit receivedNewNotifications(numNew);

        // If there's not enough to cause scrollbars, this will bunch them
        // at the top.
        ((QVBoxLayout *)m_nContainer->layout())->addStretch();
    }
    else // we already have this one
    {
        delete n;
    }
}


void NotificationCenter::getPixmap(QLabel *ul, DATA::FbPageInfo& fbp) {

    UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();

    QPixmap *p = cache->getPixmap(fbp.getPageId(), UTIL::FbUserPicCache::PicSquare,
                                  fbp.getPicSquare());
    if (p != 0)
    {
        ul->setPixmap(*p);
        delete p;
    }
    else
    {
        QNetworkReply *reply;

        QUrl url(fbp.getPicSquare());
        reply = m_nam->get(QNetworkRequest(url));

        QString s("pagePicSquare:" + fbp.getPageId());

        m_tmpMap.insert(reply, QPair<QString, QLabel *>(s, ul));
    }

}

void NotificationCenter::getPixmap(QLabel *ul, DATA::FbUserInfo& fbu) {

    UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();

    QPixmap *p = cache->getPixmap(fbu.getUID(), UTIL::FbUserPicCache::PicSquare,
                                  fbu.getPicSquare());
    if (p != 0)
    {
        ul->setPixmap(*p);
        delete p;
    }
    else
    {
        QNetworkReply *reply;

        QUrl url(fbu.getPicSquare());
        reply = m_nam->get(QNetworkRequest(url));

        QString s("userPicSquare:" + fbu.getUID());

        m_tmpMap.insert(reply, QPair<QString, QLabel *>(s, ul));
    }

}

void NotificationCenter::getPixmap(QLabel *al, DATA::AppInfo& ai) {

    if (m_iconPixmapCache.contains(ai.getAppId()))
        al->setPixmap(m_iconPixmapCache[ai.getAppId()]);
    else
    {
        QNetworkReply *reply;

        QUrl url(ai.getIconUrl());
        reply = m_nam->get(QNetworkRequest(url));

        QString s("appIcon:" + ai.getAppId());

        m_tmpMap.insert(reply, QPair<QString, QLabel *>(s, al));

    }

}

void NotificationCenter::receiveIconPixmap(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {

        QPair<QString, QLabel *> pair = m_tmpMap.take(reply);
        QLabel *a = pair.second;
        QPixmap p;
        p.loadFromData(reply->readAll());
        a->setPixmap(p);

        if (pair.first.startsWith("appIcon:"))
        {
            m_iconPixmapCache.insert(pair.first.remove("appIcon:"), p);
        }
        else if (pair.first.startsWith("userPicSquare:"))
        {
            UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
            cache->cachePixmap(pair.first.remove("userPicSquare:"), UTIL::FbUserPicCache::PicSquare,
                               reply->request().url(), p);
        }
        else if (pair.first.startsWith("pagePicSquare:"))
        {
            UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
            cache->cachePixmap(pair.first.remove("pagePicSquare:"), UTIL::FbUserPicCache::PicSquare,
                               reply->request().url(), p);
        }


    } else {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }

}

void NotificationCenter::linkActivated(QString url) {

    qDebug() << "Notification Center; Url: " + url;

    QString postId = "";

    QRegExp rx1("v=feed&story_fbid=(\\d+)&id=(\\d+)");
    QRegExp rx2("streamPost:(\\d+_\\d+)");

    if (rx1.indexIn(url) != -1)
        postId = rx1.cap(2) + "_" + rx1.cap(1);
    else if (rx2.indexIn(url) != -1)
        postId = rx2.cap(1);

    if (m_streamPosts.contains(postId))
    {
        StreamPostWidget *spw = new StreamPostWidget(m_streamPosts[postId], m_userInfo);
        connect(spw, SIGNAL(closed(GUI::StreamPostWidget*)),
                this, SLOT(streamPostClosed(GUI::StreamPostWidget*)));
        spw->show();

    }
    else
    {
       if (postId != "")
        {
            API::Method *method = m_factory->createMethod("fql.multiquery.getStreamPosts");
            method->setArgument("post_id", postId);
            bool rc = method->execute();
            if (!rc)
                qDebug() << "Method fql.multiquery.getStreamPosts error: " << method->getErrorStr();
        }
    }

    qDebug() << postId;

}

void NotificationCenter::streamPostClosed(GUI::StreamPostWidget *spw) {
    delete spw;
}

void NotificationCenter::closeEvent(QCloseEvent *event) {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    settings.setValue("NotificationCenter", saveGeometry());
    settings.endGroup();
    event->accept();
}


} // namespace GUI
