#ifndef QTFACEBOOK_H
#define QTFACEBOOK_H

#include <QObject>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QTimer>

#include "userinfo.h"
#include "fbconnectwizard.h"
#include "testqueryconsole.h"
#include "api/notifications_markread.h"

class QtFacebook : public QObject
{
Q_OBJECT
public:
    explicit QtFacebook(QObject *parent = 0);

signals:

public slots:
    void fbWizardComplete();
    void saveUserInfo(UserInfo *info);
    void fbWizardCanceled();
    void updateIcon();

private slots:
    void notificationCheck(QList<API::Notifications::Notification> *nList);
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void nextTrayIcon();
    void ackNewNotifications();
    void viewAllNotifications();
    void exitMenuAction();
    void apiNotificationsMarkRead(API::Notifications::MarkRead *method);

private:
    void showNotifications();
    bool loadUserInfo();
    UserInfo *m_userInfo;
    FBConnectWizard *m_wizard;
    TestQueryConsole *m_testConsole;
    QSystemTrayIcon *m_trayIcon;
    QList<API::Notifications::Notification> *m_notificationList;
    QIcon *m_trayIcons[4];
    bool m_traySingleClicked;
    bool m_balloonMessageClicked;
    int m_trayIconIndex;
    bool m_animatingTrayIcon;
    QTimer *m_trayAnimationTimer;
    QAction *m_notificationCountMenuAction;
    QAction *m_ackNotificationsMenuAction;
    QAction *m_viewAllNotificationsMenuAction;

};

#endif // QTFACEBOOK_H