#include "notificationcheck.h"
#include "api/factory.h"

#include <QDebug>

NotificationCheck::NotificationCheck(UserInfo *userInfo, int interval)
{
    m_userInfo = userInfo;
    m_checkInterval = interval;

    // Init the factory
    m_factory = new API::Factory(m_userInfo);

    connect(m_factory, SIGNAL(apiNotificationsGetList(API::Notifications::GetList*)),
                this, SLOT(apiNotificationsGetList(API::Notifications::GetList*)));

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

    API::Method *method = m_factory->createMethod("notifications.getList");

    bool rc = method->execute();
    if (!rc)
        qDebug() << method->getErrorStr();

}

void NotificationCheck::apiNotificationsGetList(API::Notifications::GetList *method) {

    qDebug() << "apiNotificationsGetList()";


    QList<API::Notifications::Notification> list;
    list = method->getNotifications();

    qDebug() << "list.size(): " << list.size();

    QList<API::Notifications::Notification> *nList = new QList<API::Notifications::Notification>;

    while (!list.empty())
    {
        API::Notifications::Notification n = list.takeFirst();
        if (n.getIsRead() == false && n.getIsHidden() == false)
            nList->prepend(n);
    }

    qDebug() << "nList->size(): " << nList->size();


    emit newNotifications(nList);

    delete method;

}

void NotificationCheck::setCheckInterval(int minutes) {
    m_timer->stop();
    m_timer->start(minutes * 60000);
}