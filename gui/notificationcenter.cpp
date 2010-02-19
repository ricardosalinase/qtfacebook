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
    m_userInfo(userInfo)
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

    StreamPostWidget *spw = new StreamPostWidget(list->takeAt(0));
    spw->show();
    spw->scrollToBottom();

    delete list;
    method->deleteLater();

}


void NotificationCenter::getStreamPostsFailed(API::FQL::GetStreamPosts *method) {
    delete method;
}

void NotificationCenter::notificationAcknowledged(QString nId) {

    // Mark the notification read via the API
    API::Method *method = m_factory->createMethod("notifications.markRead");

    method->setArgument("notification_ids",nId);
    method->execute();



}

void NotificationCenter::notificationsMarkedAsRead(API::Notifications::MarkRead *method) {


    QString nId = method->getNotificationIds();
    emit acknowledgedNotification(nId);
    delete method;

}
void NotificationCenter::notificationsMarkedAsReadFailed(API::Notifications::MarkRead *method) {
    qDebug() << "notifications.markRead failed, resending.";
    QString nId = method->getNotificationIds();
    delete method;
    notificationAcknowledged(nId);
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

        GUI::NotificationLabel *nl = new GUI::NotificationLabel(n);
        GUI::AppInfoLabel *ai = new GUI::AppInfoLabel(n->getAppInfo());
        getPixmap(ai);
        nWidget = new GUI::NotificationCenterWidget(nl, ai);
        connect(nWidget, SIGNAL(linkActivated(QString)),
                this, SLOT(linkActivated(QString)));
        connect(nWidget, SIGNAL(acknowledged(QString)),
                this, SLOT(notificationAcknowledged(QString)));
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


void NotificationCenter::getPixmap(GUI::AppInfoLabel *ai) {

    if (m_iconPixmapCache.contains(ai->getAppInfo().getAppId()))
        ai->myIconPixmap(m_iconPixmapCache[ai->getAppInfo().getAppId()], true);
    else {
        QNetworkAccessManager *m_nam = new QNetworkAccessManager();
        QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(receiveIconPixmap(QNetworkReply*)));

        QNetworkReply *reply;

        QUrl url(ai->getAppInfo().getIconUrl());
        reply = m_nam->get(QNetworkRequest(url));
        m_tmpMap.insert(reply, ai);

    }

}

void NotificationCenter::receiveIconPixmap(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {

        GUI::AppInfoLabel *a = m_tmpMap.take(reply);
        QPixmap *p = new QPixmap();
        p->loadFromData(reply->readAll());
        a->myIconPixmap(p, true);
        m_iconPixmapCache.insert(a->getAppInfo().getAppId(), p);

    } else {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }

}

void NotificationCenter::linkActivated(QString url) {

    qDebug() << "Notification Center; Url: " + url;

    QString postId;
    QRegExp rx("v=feed&story_fbid=(\\d+)&id=(\\d+)");
    if (rx.indexIn(url) != -1)
    {
        postId = rx.cap(2) + "_" + rx.cap(1);
        qDebug() << postId;
        API::Method *method = m_factory->createMethod("fql.multiquery.getStreamPosts");
        method->setArgument("post_id", postId);
        bool rc = method->execute();
        if (!rc)
            qDebug() << "Method fql.multiquery.getStreamPosts error: " << method->getErrorStr();
    }
}

void NotificationCenter::closeEvent(QCloseEvent *event) {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    settings.setValue("NotificationCenter", saveGeometry());
    settings.endGroup();
    event->accept();
}


} // namespace GUI
