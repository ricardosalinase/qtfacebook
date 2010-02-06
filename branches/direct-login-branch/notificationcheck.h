#ifndef NOTIFICATIONCHECK_H
#define NOTIFICATIONCHECK_H

#include <QThread>
#include <QList>
#include <QTimer>
#include <QMap>

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
    void newNotifications(QList<DATA::Notification *> *nList, QMap<QString, DATA::AppInfo *> *aMap);
    void sessionExpired();

protected:
    void run();

private slots:
    void checkForNotifiations();
    void apiFqlGetNewNotifications(API::FQL::GetNewNotifications*);
    void apiFqlGetAppInfo(API::FQL::GetAppInfo*);

private:
    UserInfo *m_userInfo;
    int m_checkInterval;
    API::Factory *m_factory;
    QTimer *m_timer;
    QList<DATA::Notification*> *m_notificationList;

    uint m_lastNotificationCheck;

};

#endif // NOTIFICATIONCHECK_H
