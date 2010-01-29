#ifndef QTFACEBOOK_H
#define QTFACEBOOK_H

#include <QObject>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QTimer>

#include "userinfo.h"
#include "fbconnectwizard.h"
#include "testqueryconsole.h"

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
    void balloonMessageClicked();
    void nextTrayIcon();

private:
    UserInfo *m_userInfo;
    bool loadUserInfo();
    FBConnectWizard *m_wizard;
    TestQueryConsole *m_testConsole;
    QSystemTrayIcon *m_trayIcon;
    QList<API::Notifications::Notification> *m_notificationList;
    QIcon *m_trayIcons[4];
    bool m_traySingleClicked;
    bool m_balloonMessageClicked;
    void showNotificationBalloon();
    int m_trayIconIndex;
    bool m_animatingTrayIcon;
    QTimer *m_trayAnimationTimer;


};

#endif // QTFACEBOOK_H
