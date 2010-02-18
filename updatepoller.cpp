#include "updatepoller.h"

UpdatePoller::UpdatePoller(UserInfo *info, QObject *parent) :
        Worker(parent),
        m_userInfo(info),
        m_notificationUpdateInterval(1),
        m_lastNotificationTime("0")
{
}


void UpdatePoller::init() {

    m_factory = new API::Factory(m_userInfo, this);

    m_notificationTimer = new QTimer(this);
    connect(m_notificationTimer, SIGNAL(timeout()),
            this, SLOT(getNewNotifications()));
    m_notificationTimer->start(1000 * (m_notificationUpdateInterval * 60));



}

void UpdatePoller::getNewNotifications() {

    API::FQL::GetNewNotifications *method = m_factory->createMethod("fql.multiquery.getNewNotifications");

    method->setArgument("only_unread",1);
    method->setArgument("start_time",m_lastNotificationTime);

    bool rc = method->execute();
    if (!rc)
    {
        qDebug() << "fql.multiquery.getNewNotifications exec() failed in UpdatePoller::getNewNotifications(): " << method->errorString();
        delete method;
    }

}
