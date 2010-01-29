#ifndef QTFACEBOOK_H
#define QTFACEBOOK_H

#include <QObject>
#include <QVBoxLayout>
#include <QSystemTrayIcon>

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

private:
    UserInfo *m_userInfo;
    bool loadUserInfo();
    FBConnectWizard *m_wizard;
    TestQueryConsole *m_testConsole;
    QSystemTrayIcon *m_trayIcon;
};

#endif // QTFACEBOOK_H
