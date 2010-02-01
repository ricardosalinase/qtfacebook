#ifndef NOTIFICATIONCHECK_H
#define NOTIFICATIONCHECK_H

#include <QThread>
#include <QList>
#include <QTimer>

#include "api/factory.h"


class NotificationCheck : public QThread
{
    Q_OBJECT

public:
    NotificationCheck(UserInfo *userInfo, int interval);
    ~NotificationCheck();

    void setUserInfo(UserInfo *);
    void setCheckInterval(int minutes);

signals:
    void newNotifications(QList<DATA::Notification*> *nList);
    void sessionExpired();

protected:
    void run();

private slots:
    void checkForNotifiations();
    void apiNotificationsGetList(API::Notifications::GetList*);

private:
    UserInfo *m_userInfo;
    int m_checkInterval;
    API::Factory *m_factory;
    QTimer *m_timer;

};

#endif // NOTIFICATIONCHECK_H
