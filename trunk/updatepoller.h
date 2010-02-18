#ifndef UPDATEPOLLER_H
#define UPDATEPOLLER_H

#include <QTimer>

#include "userinfo.h"
#include "util/worker.h"
#include "api/factory.h"

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

public slots:

private slots:
    void getNewNotifications();

private:
    UserInfo *m_userInfo;
    API::Factory *m_factory;
    QTimer *m_notificationTimer;
    int m_notificationUpdateInterval;
    QString m_lastNotificationTime;
};



#endif // UPDATEPOLLER_H
