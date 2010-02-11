#include <QSettings>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QNetworkReply>
#include <QApplication>
#include <QDesktopWidget>
#include <QObject>

#include "api/factory.h"
#include "notificationcenter.h"
#include "appinfolabel.h"
#include "notificationlabel.h"
#include "notificationcenterwidget.h"


namespace GUI {

NotificationCenter::NotificationCenter(UserInfo *userInfo, QWidget *parent) :
    QWidget(parent),
    m_startup(true),
    m_userInfo(userInfo),
    m_notificationList(0)
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

    m_factory = new API::Factory(m_userInfo);
    connect(m_factory, SIGNAL(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)),
            this, SLOT(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetAppInfo(API::FQL::GetAppInfo*)),
            this, SLOT(apiFqlGetAppInfo(API::FQL::GetAppInfo*)));

    getInitialNotifications();

}

void NotificationCenter::getInitialNotifications() {

    API::Method *method = m_factory->createMethod("fql.query.getNewNotifications");
    method->setArgument("start_time", 0);
    method->setArgument("only_unread",1);

    bool rc = method->execute();
    if (!rc)
        qDebug() << method->getErrorStr();

}

void NotificationCenter::apiFqlGetNewNotifications(API::FQL::GetNewNotifications *method) {

    QList<DATA::Notification*> *list = method->getNotificationList();

    if (m_notificationList == 0)
        m_notificationList = new QList<DATA::Notification*>;

    if (list->size() > 0) {

        QStringList appIds;

        while (!list->empty())
        {
            DATA::Notification *n = list->takeFirst();
            m_notificationList->prepend(n);
            appIds.append(n->getAppId());
        }

        // Now get the App Icons
        API::Method *method2 = m_factory->createMethod("fql.query.getAppInfo");
        method2->setArgument("app_ids", appIds);
        bool rc = method2->execute();
        if (!rc)
            qDebug() << method2->getErrorStr();

    }

    if (m_notificationList->size() < 10 && m_startup == true)
    {
        // I think we want to pre-fill the window so it's not empty
        // The downside is that it's only notifications with no feed
        // posts. m_startup is a safeguard against the user having
        // < 10 notifications total available.
        m_startup = false;
        API::Method *method2 = m_factory->createMethod("fql.query.getNewNotifications");
        method2->setArgument("start_time", 0);
        method2->setArgument("limit", QString::number(10 - m_notificationList->size()));

        bool rc = method2->execute();
        if (!rc)
            qDebug() << method2->getErrorStr();

    }

    delete list;
    delete method;

}

void NotificationCenter::apiFqlGetAppInfo(API::FQL::GetAppInfo *method) {

    newNotifications(m_notificationList, method->getAppInfoMap());
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

void NotificationCenter::navigate(QUrl url) {
    qDebug() << "Url: " + url.toString();
}

void NotificationCenter::notificationAcknowledged(QString nId) {

    // Mark the notification read via the API
    API::Factory *factory = new API::Factory(m_userInfo);
    API::Method *method = factory->createMethod("notifications.markRead");
    connect(factory, SIGNAL(apiNotificationsMarkRead(API::Notifications::MarkRead*)),
            this, SLOT(notificationsMarkedAsRead(API::Notifications::MarkRead*)));

    method->setArgument("notification_ids",nId);
    method->execute();



}

void NotificationCenter::notificationsMarkedAsRead(API::Notifications::MarkRead *method) {

    QString nId = method->getNotificationIds();

    if (method->successful()) {
        //emit acknowledgedNotification(nId);
        // no op - it'll come back via the comet connection
    } else {
        qDebug() << "notifications.markRead failed, resending.";
        notificationAcknowledged(nId);
    }

    delete method;

}

void NotificationCenter::deactivateNotification(QString nid) {

    for (int i = 0; i < m_newNotifications.size(); i++) {
        // There's a case where facebook sends out a null list of ids,
        // meaning they've all been read. We signify this by sending "-1" as
        // the notificationID from the CometConnector.
        if (nid.compare("-1") == 0 || m_newNotifications.at(i)->getNotificationId().compare(nid) == 0) {
            m_newNotifications.at(i)->stopAfter(5);
            m_newNotifications.takeAt(i);
            break;
        }
    }

    emit acknowledgedNotification(nid);

}

void NotificationCenter::newNotifications(QList<DATA::Notification *> *nList, QMap<QString, DATA::AppInfo *> *aMap) {

    qDebug() << "newNotifications(); nList: " << nList->size();
    while (!nList->empty())
    {
        DATA::Notification *n = nList->takeFirst();
        DATA::AppInfo *a = new DATA::AppInfo(*(aMap->value(n->getAppId())));
        newNotification(n,a);
    }

    AppInfo *tmp;
    QMap<QString, AppInfo *>::iterator i = aMap->begin();
     while (i != aMap->end()) {
             tmp = i.value();
             ++i;
             delete tmp;
     }

     delete nList;
     delete aMap;


}

void NotificationCenter::newNotification(DATA::Notification *n, DATA::AppInfo *a) {

    GUI::NotificationCenterWidget *nWidget;

    int numNew = 0;
    GUI::NotificationLabel *nl = new GUI::NotificationLabel(n);
    GUI::AppInfoLabel *ai = new GUI::AppInfoLabel(a);
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
        m_newNotifications.append(nWidget);
        nWidget->start();
    }

    if (numNew != 0)
        emit receivedNewNotifications(numNew);

    // If there's not enough to cause scrollbars, this will bunch them
    // at the top.
    ((QVBoxLayout *)m_nContainer->layout())->addStretch();
}


void NotificationCenter::getPixmap(GUI::AppInfoLabel *ai) {

    if (m_iconPixmapCache.contains(ai->getAppInfo()->getAppId()))
        ai->myIconPixmap(m_iconPixmapCache[ai->getAppInfo()->getAppId()], true);
    else {
        QNetworkAccessManager *m_nam = new QNetworkAccessManager();
        QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(receiveIconPixmap(QNetworkReply*)));

        QNetworkReply *reply;

        QUrl url(ai->getAppInfo()->getIconUrl());
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
        m_iconPixmapCache.insert(a->getAppInfo()->getAppId(), p);

    } else {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }

}

void NotificationCenter::linkActivated(QString url) {
    qDebug() << url;
}

void NotificationCenter::closeEvent(QCloseEvent *event) {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    settings.setValue("NotificationCenter", saveGeometry());
    settings.endGroup();
    event->accept();
}


} // namespace GUI
