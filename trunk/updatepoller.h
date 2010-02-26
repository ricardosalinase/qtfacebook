#ifndef UPDATEPOLLER_H
#define UPDATEPOLLER_H

#include <QTimer>
#include <QList>

#include "userinfo.h"
#include "util/worker.h"
#include "api/factory.h"
#include "data/notification.h"
#include "data/streampost.h"

/// The UpdatePoller class is a UTIL::Worker that continuously polls the various tables at Facebook
/// looking for new things. It produces the objects and sends them out via signals
class UpdatePoller : public UTIL::Worker
{
    Q_OBJECT
public:
    UpdatePoller(UserInfo *info, QObject *parent = 0);
    void init();
    void setStreamPostInterval(int updateTime);
    void setNotificationInterval(int updateTime);

signals:
    void apiFqlGetNewNotifications(QList<DATA::Notification*> *);
    void apiFqlGetStreamPosts(QList<DATA::StreamPost*> *);

public slots:

private slots:
    void getNewNotifications();
    void gotNewNotifications(API::FQL::GetNewNotifications *method);
    void gotNewNotificationsFailed(API::FQL::GetNewNotifications *method);
    void getNewStreamPosts();
    void gotNewStreamPosts(API::FQL::GetStreamPosts *method);
    void gotNewStreamPostsFailed(API::FQL::GetStreamPosts *method);

private:
    UserInfo *m_userInfo;
    API::Factory *m_factory;
    QTimer *m_notificationTimer;
    QTimer *m_streamPostTimer;
    int m_notificationUpdateInterval;
    int m_streamPostUpdateInterval;
    QString m_lastStreamPostCheck;
};



#endif // UPDATEPOLLER_H
