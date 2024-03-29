#ifndef QTFACEBOOK_H
#define QTFACEBOOK_H

#include <QObject>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QTimer>

#include "fbconnectwizard.h"
#include "testqueryconsole.h"
#include "api/notifications_MarkRead.h"
#include "gui/notificationcenter.h"
#include "gui/notificationcenteritem.h"
#include "util/workerthread.h"
#include "updatepoller.h"
#include "api/factory.h"

class QtFacebook : public QObject
{
Q_OBJECT
public:
    explicit QtFacebook(QObject *parent = 0);

signals:

public slots:
    void start();
    void fbWizardComplete();
    void saveUserInfo();
    void fbWizardCanceled();
    void updateIcon();

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void nextTrayIcon();
    void exitMenuAction();
    void testQueryConsole();
    void receivedNewNotifications(int numNew);
    void receivedNewStreamPosts(int numNew);
    void acknowledgedNotification(GUI::NotificationCenterItem::ItemType type, QString nid);
    void apiUsersGetLoggedInUser(API::Users::GetLoggedInUser *);
    void apiUsersGetLoggedInUserFailed(API::Users::GetLoggedInUser *);

private:
    void showNotifications(bool showBalloonMessage);
    bool loadUserInfo();
    FBConnectWizard *m_wizard;
    TestQueryConsole *m_testConsole;
    GUI::NotificationCenter *m_notificationCenter;
    UpdatePoller *m_updatePoller;
    QSystemTrayIcon *m_trayIcon;
    QList<DATA::Notification *> *m_notificationList;
    QIcon *m_trayIcons[4];
    bool m_traySingleClicked;
    bool m_balloonMessageClicked;
    int m_trayIconIndex;
    bool m_animatingTrayIcon;
    QTimer *m_trayAnimationTimer;
    QByteArray m_geometry;
    int m_totalNotifications;
    int m_standardNotifications;
    int m_streamPostNotifications;
    UTIL::WorkerThread *m_updateThread;
};

#endif // QTFACEBOOK_H
