#include <QSettings>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QNetworkReply>
#include <QApplication>
#include <QDesktopWidget>
#include <QObject>

#include "notificationcenter.h"
#include "appinfolabel.h"
#include "notificationlabel.h"
#include "notificationcenterwidget.h"


namespace GUI {

NotificationCenter::NotificationCenter(UserInfo *userInfo, QWidget *parent) :
    QWidget(parent),
    m_userInfo(userInfo),
    m_showHiddenNotifications(false)
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

    m_cometConnector = new CometConnector(m_userInfo);
    connect(m_cometConnector, SIGNAL(newNotification(DATA::Notification*,DATA::AppInfo*)),
            this, SLOT(newNotification(DATA::Notification*,DATA::AppInfo*)),
            Qt::QueuedConnection);
    connect(m_cometConnector, SIGNAL(notificationAck(QString)),
            this, SLOT(deactivateNotification(QString)),
            Qt::QueuedConnection);


    m_cometConnector->start();

    m_notificationCheck = new NotificationCheck(m_userInfo,1);
    connect(m_notificationCheck, SIGNAL(newNotifications(QList<DATA::Notification*>*,QMap<QString,DATA::AppInfo*>*)),
            this, SLOT(newNotifications(QList<DATA::Notification*>*,QMap<QString,DATA::AppInfo*>*)),
            Qt::QueuedConnection);

    m_notificationCheck->start();




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

//void NotificationCenter::changeEvent(QEvent *event) {
//    qDebug() << "actionEvent()" << event->type();
//}

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
        if (m_newNotifications.at(i)->getNotificationId().compare(nid) == 0) {
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
    m_newNotifications.append(nWidget);

    if (n->getIsHidden() && !m_showHiddenNotifications)
        nWidget->hide();
    else {
        numNew++;
        nWidget->start();
    }

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
