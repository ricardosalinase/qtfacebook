#include "notificationcheck.h"
#include "api/factory.h"

#include <QDebug>
#include <QDateTime>

NotificationCheck::NotificationCheck(UserInfo *userInfo, int interval) :
        m_lastNotificationCheck(0)
{
    m_userInfo = userInfo;
    m_checkInterval = interval;

    // Init the factory
    m_factory = new API::Factory(m_userInfo);

    connect(m_factory, SIGNAL(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)),
            this, SLOT(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetAppInfo(API::FQL::GetAppInfo*)),
            this, SLOT(apiFqlGetAppInfo(API::FQL::GetAppInfo*)));


}

NotificationCheck::~NotificationCheck() {
    m_timer->stop();
    delete m_timer;
}

void NotificationCheck::setUserInfo(UserInfo *userInfo) {
    m_userInfo = userInfo;
    m_factory->setUserInfo(m_userInfo);
}

void NotificationCheck::run() {


    QTimer::singleShot(400, this, SLOT(checkForNotifiations()));

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(checkForNotifiations()));
    m_timer->start(m_checkInterval * 60000);


    exec();

}

void NotificationCheck::checkForNotifiations() {

    qDebug() << "checkForNotifications()";

    API::Method *method = m_factory->createMethod("fql.query.getNewNotifications");



    method->setArgument("start_time", QString::number(m_lastNotificationCheck));
    method->setArgument("only_unread",1);


    bool rc = method->execute();
    if (!rc)
        qDebug() << method->getErrorStr();

     //m_lastNotificationCheck = QDateTime::currentDateTime().toUTC().toTime_t();

}

void NotificationCheck::apiFqlGetNewNotifications(API::FQL::GetNewNotifications *method) {

    qDebug() << "apiFqlGetNewNotificaitons()";


    QList<DATA::Notification*> *list = method->getNotificationList();

    qDebug() << "list->size(): " << list->size();


    // If there's no new notifications ... we're done
    if (list->size() > 0) {

        m_notificationList = new QList<DATA::Notification*>;
        QStringList appIds;


        while (!list->empty())
        {
            DATA::Notification *n = list->takeFirst();
            m_notificationList->prepend(n);
            appIds.append(n->getAppId());
        }

        // This syncs us with facebook's time rather than ours.
        // Solves some timing issues I was seeing
        m_lastNotificationCheck = m_notificationList->last()->getCreatedTime().toUInt();


        qDebug() << "m_motificationList->size(): " << m_notificationList->size();

        // Now get the App Icons
        API::Method *method2 = m_factory->createMethod("fql.query.getAppInfo");
        method2->setArgument("app_ids", appIds);
        bool rc = method2->execute();
        if (!rc)
            qDebug() << method2->getErrorStr();

    }

    delete list;
    delete method;

    //emit newNotifications(nList, method->getAppInfoMap());


}

void NotificationCheck::apiFqlGetAppInfo(API::FQL::GetAppInfo *method) {
    qDebug() << "apiFqlGetAppInfo()";

    emit newNotifications(m_notificationList, method->getAppInfoMap());
    delete method;
}

void NotificationCheck::setCheckInterval(int minutes) {
    m_timer->stop();
    m_timer->start(minutes * 60000);
}
