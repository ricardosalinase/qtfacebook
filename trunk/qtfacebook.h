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
#include "gui/notifications_listview.h"
#include "gui/notificationcenter.h"
#include "gui/facebooklogindialog.h"


class QtFacebook : public QObject
{
Q_OBJECT
public:
    explicit QtFacebook(QObject *parent = 0);

signals:
    void newChatMessage(DATA::ChatMessage *);
    void getBuddyList();

public slots:
    void fbWizardComplete();
    void saveUserInfo();
    void fbWizardCanceled();
    void updateIcon();

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void nextTrayIcon();
    void viewAllNotifications();
    void viewRecentNotifications();
    void viewNewNotifications();
    void exitMenuAction();
    void testQueryConsole();
    void receivedNewNotifications(int numNew);
    void acknowledgedNotification(QString nid);
    void gotLoginResults(bool);
    void gotLoginInfo();
    void loginCanceled();
    void loginFailedDialogClosed();
    void newChatMessageReceived(DATA::ChatMessage *c);

private:
    void viewNotifications(GUI::Notifications::ListView::mode m);
    void showNotifications(bool showBalloonMessage);
    bool loadUserInfo();
    UserInfo *m_userInfo;
    FBConnectWizard *m_wizard;
    TestQueryConsole *m_testConsole;
    GUI::NotificationCenter *m_notificationCenter;
    CometConnector *m_cometConnector;
    QSystemTrayIcon *m_trayIcon;
    GUI::Notifications::ListView *m_notificationListView;
    QList<DATA::Notification *> *m_notificationList;
    QIcon *m_trayIcons[4];
    bool m_traySingleClicked;
    bool m_balloonMessageClicked;
    int m_trayIconIndex;
    bool m_animatingTrayIcon;
    QTimer *m_trayAnimationTimer;
    QAction *m_notificationCountMenuAction;
    QAction *m_viewAllNotificationsMenuAction;
    QByteArray m_geometry;
    int m_totalNotifications;
    int m_standardNotifications;
    QDialog *m_invalidLogin;
    GUI::FacebookLoginDialog *m_loginDialog;

};

#endif // QTFACEBOOK_H
