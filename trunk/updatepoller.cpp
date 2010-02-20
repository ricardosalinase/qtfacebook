#include "updatepoller.h"

#include <QDebug>
#include <QList>
#include <QDateTime>

UpdatePoller::UpdatePoller(UserInfo *info, QObject *parent) :
        Worker(parent),
        m_userInfo(info),
        m_notificationUpdateInterval(1),
        m_streamPostUpdateInterval(1)
{
}




void UpdatePoller::init() {

    m_factory = new API::Factory(m_userInfo, this);

    connect(m_factory, SIGNAL(apiFqlGetNewNotifications(API::FQL::GetNewNotifications*)),
            this, SLOT(gotNewNotifications(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetNewNotificationsFailed(API::FQL::GetNewNotifications*)),
            this, SLOT(gotNewNotificationsFailed(API::FQL::GetNewNotifications*)));
    connect(m_factory, SIGNAL(apiFqlGetStreamPostInfo(API::FQL::GetStreamPostInfo*)),
            this, SLOT(gotNewStreamPostInfo(API::FQL::GetStreamPostInfo*)));
    connect(m_factory, SIGNAL(apiFqlGetStreamPostInfoFailed(API::FQL::GetStreamPostInfo*)),
            this, SLOT(gotNewStreamPostInfoFailed(API::FQL::GetStreamPostInfo*)));


    m_notificationTimer = new QTimer(this);
    connect(m_notificationTimer, SIGNAL(timeout()),
            this, SLOT(getNewNotifications()));
    m_notificationTimer->start(1000 * (m_notificationUpdateInterval * 60));

    m_lastStreamPostCheck = QString::number(QDateTime::currentDateTime().toUTC().toTime_t());
    //m_lastStreamPostCheck = "0";

    m_streamPostTimer = new QTimer(this);
    connect(m_streamPostTimer, SIGNAL(timeout()),
            this, SLOT(getNewStreamPosts()));
    m_streamPostTimer->start(1000 * (m_streamPostUpdateInterval * 60));


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

    API::Method *method = m_factory->createMethod("fql.multiquery.getStreamPostInfo");
    method->setArgument("start_time", m_lastStreamPostCheck);
    method->setArgument("get_hidden",1);
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "fql.multiquery.getStreamPostInfo exec() failed in UpdatePoller::getNewStreamPosts(): " << method->errorString();
        delete method;
    }

}

void UpdatePoller::gotNewStreamPostInfo(API::FQL::GetStreamPostInfo *method) {

    QList<DATA::StreamPost *> *pList = method->getStreamPosts();
    qDebug() << "UpdatePoller::gotNewStreamPostInfo(); pList:" << pList->size();

    if (pList->size())
    {
        // This syncs us with their time
        m_lastStreamPostCheck = pList->first()->getCreatedTime();

        emit apiFqlGetStreamPostInfo(pList);
    }
    else
    {
        delete pList;
    }

    delete method;

}

void UpdatePoller::gotNewStreamPostInfoFailed(API::FQL::GetStreamPostInfo *method) {
    delete method;
    getNewStreamPosts();

}


void UpdatePoller::setStreamPostInterval(int updateTime) {
    m_streamPostUpdateInterval = updateTime;
}

void UpdatePoller::setNotificationInterval(int updateTime) {
    m_notificationUpdateInterval = updateTime;
}


