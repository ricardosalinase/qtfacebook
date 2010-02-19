#include "updatepoller.h"

#include <QDebug>
#include <QList>
#include <QDateTime>

UpdatePoller::UpdatePoller(UserInfo *info, QObject *parent) :
        Worker(parent),
        m_userInfo(info),
        m_notificationUpdateInterval(1),
        m_streamPostUpdateInterval(5)
{
}




void UpdatePoller::init() {

    m_factory = new API::Factory(m_userInfo, this);

    connect(m_factory, SIGNAL(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)),
            this, SLOT(gotNewNotifications(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetNewNotificationsFailed(API::FQL::GetNewNotifications*)),
            this, SLOT(gotNewNotificationsFailed(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetStreamPosts(API::FQL::GetStreamPosts*)),
            this, SLOT(gotNewStreamPosts(API::FQL::GetStreamPosts*)));
    connect(m_factory, SIGNAL(apiFqlGetStreamPostsFailed(API::FQL::GetStreamPosts*)),
            this, SLOT(gotNewStreamPostsFailed(API::FQL::GetStreamPosts*)));


    m_notificationTimer = new QTimer(this);
    connect(m_notificationTimer, SIGNAL(timeout()),
            this, SLOT(getNewNotifications()));
    m_notificationTimer->start(1000 * (m_notificationUpdateInterval * 60));

    m_lastStreamPostCheck = QDateTime::currentDateTime().toTime_t();

    m_streamPostTimer = new QTimer(this);
    connect(m_streamPostTimer, SIGNAL(timeout()),
            this, SLOT(getNewStreamPosts()));
    m_streamPostTimer->start(5000 * (m_streamPostUpdateInterval * 60));


    QTimer::singleShot(1000,this,SLOT(getNewNotifications()));


}

void UpdatePoller::getNewNotifications() {

    API::Method *method = m_factory->createMethod("fql.multiquery.getNewNotifications");

    method->setArgument("only_unread",1);
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "fql.multiquery.getNewNotifications exec() failed in UpdatePoller::getNewNotifications(): " << method->errorString();
        delete method;
    }

}

void UpdatePoller::gotNewNotifications(API::FQL::GetNewNotifications *method) {

    QList<DATA::Notification *> *nList = method->getNotificationList();
    emit apiFqlGetNewNotifications(nList);

    delete method;



}

void UpdatePoller::gotNewNotificationsFailed(API::FQL::GetNewNotifications *method) {

    delete method;
    getNewNotifications();
}

void UpdatePoller::getNewStreamPosts() {



}

void UpdatePoller::gotNewStreamPosts(API::FQL::GetStreamPosts *method) {

}

void UpdatePoller::gotNewStreamPostsFailed(API::FQL::GetStreamPosts *method) {

}


void UpdatePoller::setStreamPostInterval(int updateTime) {
    m_streamPostUpdateInterval = updateTime;
}

void UpdatePoller::setNotificationInterval(int updateTime) {
    m_notificationUpdateInterval = updateTime;
}


