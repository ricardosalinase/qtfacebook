#include <QDebug>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QTimer>
#include <QMatrix>
#include <QMenu>
#include <QApplication>

#include "qtfacebook.h"
#include "fbconnectwizard.h"
#include "testqueryconsole.h"
#include "api/factory.h"
#include "notificationcheck.h"

#define API_KEY "61cecf6f7ee5528d294e1d6bf675f424"

QtFacebook::QtFacebook(QObject *parent) :
    QObject(parent),
    m_userInfo(0),
    m_wizard(0),
    m_testConsole(0),
    m_notificationListView(0),
    m_notificationList(0),
    m_traySingleClicked(false),
    m_balloonMessageClicked(false),
    m_trayIconIndex(0),
    m_animatingTrayIcon(false)

{

    QApplication::setQuitOnLastWindowClosed(false);

    // load session_key, uid, and secret
    bool hasInfo = loadUserInfo();

    if (!hasInfo) {
        // If we don't have those, launch the connector
        m_wizard = new FBConnectWizard(API_KEY, "qtFacebook");

        connect(m_wizard, SIGNAL(userHasAuthenticated(UserInfo*)),
                this, SLOT(saveUserInfo(UserInfo*)));

        connect(m_wizard, SIGNAL(accepted()),
                this, SLOT(fbWizardComplete()));
        connect(m_wizard, SIGNAL(rejected()),
                this, SLOT(fbWizardCanceled()));

        m_wizard->show();

    } else {
        fbWizardComplete();
    }

}

void QtFacebook::saveUserInfo(UserInfo *info) {
    m_userInfo = info;

    qDebug() << "Session Key: " << info->getSessionKey() <<
            "Secret: " << info->getSecret() <<
            "UID" << info->getUID();

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("userInfo");
    settings.setValue("SessionKey",info->getSessionKey());
    settings.setValue("UID", info->getUID());
    settings.setValue("Secret", info->getSecret());
    settings.endGroup();


}

/*************************************************************
* There is a chance that the "permanent" session from facebook
* has expired (They don't seem to understand the word
* "permanent") ... there's an API call to validate the session
* which we'll probably want to do here once we have the API
* interface object
**************************************************************/
bool QtFacebook::loadUserInfo() {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("userInfo");
    QString sKey(settings.value("SessionKey","").toString());
    QString uid(settings.value("UID","").toString());
    QString secret(settings.value("Secret","").toString());
    settings.endGroup();

    if (sKey.compare("") == 0 ||
            uid.compare("") == 0 ||
            secret.compare("") == 0)
        return false;

    if (m_userInfo != 0)
        delete m_userInfo;

    m_userInfo = new UserInfo(sKey, secret, uid, API_KEY);

    qDebug() << "Session Key: " << m_userInfo->getSessionKey() <<
            "Secret: " << m_userInfo->getSecret() <<
            "UID" << m_userInfo->getUID();


    return true;

}


void QtFacebook::fbWizardComplete() {


    // Create the context menu and system tray icon
    QPixmap *tmp;
    QMatrix m;


    for (int i = 0, r = 0; i < 4; i++, r+=45) {
        tmp = new QPixmap();
        tmp->load(":facebookIcon");
        QTransform tf;
        tf.rotate(r,Qt::ZAxis);
        m_trayIcons[i] = new QIcon(tmp->transformed(tf));
        delete tmp;
    }

    m_notificationCenter = new GUI::NotificationCenter(m_userInfo);

    m_trayIcon = new QSystemTrayIcon(*m_trayIcons[0]);

    QMenu *menu = new QMenu();

    QAction *tqc = menu->addAction("Test Query Console");

    menu->addSeparator();
    QAction *ncr = menu->addAction("Notification Center");
    menu->addSeparator();
    m_notificationCountMenuAction = menu->addAction("You have 0 new notifications");
    m_notificationCountMenuAction->setDisabled(true);
    m_ackNotificationsMenuAction = menu->addAction("Mark as read");
    m_ackNotificationsMenuAction->setDisabled(true);
    QAction *act = menu->addAction("View Recent");
    m_viewAllNotificationsMenuAction = menu->addAction("View all");
    menu->addSeparator();
    QAction *exit = menu->addAction("Exit");

    connect(tqc, SIGNAL(triggered()),
            this, SLOT(testQueryConsole()));

    connect(exit, SIGNAL(triggered()),
            this, SLOT(exitMenuAction()));
    connect(m_ackNotificationsMenuAction, SIGNAL(triggered()),
            this, SLOT(ackNewNotifications()));
    connect(m_viewAllNotificationsMenuAction, SIGNAL(triggered()),
            this, SLOT(viewAllNotifications()));
    connect(act, SIGNAL(triggered()),
            this, SLOT(viewRecentNotifications()));

    connect(ncr, SIGNAL(triggered()),
            m_notificationCenter, SLOT(showYourself()));

    m_trayIcon->setContextMenu(menu);
    m_trayIcon->show();

    if (!QSystemTrayIcon::supportsMessages())
        qDebug() << "Awwww ... bummer. It doesn't support messages";

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

    QTimer::singleShot(200, this, SLOT(updateIcon()));

    m_trayAnimationTimer = new QTimer();
    connect(m_trayAnimationTimer, SIGNAL(timeout()),
            this, SLOT(nextTrayIcon()));


}



void QtFacebook::updateIcon() {
    m_trayIcon->showMessage("Startup","You've started qtFacebook!",QSystemTrayIcon::Information);
}

void QtFacebook::trayActivated(QSystemTrayIcon::ActivationReason reason) {

    qDebug() << "trayActivated()";
    if (reason == QSystemTrayIcon::Trigger ) {
        if (!m_trayIcon->contextMenu()->isVisible())
            m_trayIcon->contextMenu()->exec(QCursor::pos());
    }
}


void QtFacebook::notificationCheck(QList<DATA::Notification *> *nList) {

    delete m_notificationList;
    m_notificationList = nList;

    if (m_notificationList->size() != 0) {
        showNotifications(m_notificationList->size() );
        if (!m_trayAnimationTimer->isActive())
            m_trayAnimationTimer->start(250);
    }
    else
    {
        m_trayAnimationTimer->stop();
        m_trayIcon->setIcon(*m_trayIcons[0]);
        m_ackNotificationsMenuAction->setEnabled(false);
        m_notificationCountMenuAction->setText("You have 0 new notifications");
        m_notificationCountMenuAction->setEnabled(false);

    }

}

void QtFacebook::showNotifications(int numNew)
{

    QString s = "You have "  + QString::number(numNew)  + " new notificaions!";
    m_trayIcon->showMessage("New Notifications", s,QSystemTrayIcon::Information, 15000);
    m_trayIcon->setToolTip(s);
    m_ackNotificationsMenuAction->setEnabled(true);
    m_notificationCountMenuAction->setText("View "
                                           + QString::number(numNew)
                                           + " new notifications)");
    m_notificationCountMenuAction->setEnabled(true);
}

void QtFacebook::fbWizardCanceled() {
    exit(0);
}

void QtFacebook::nextTrayIcon() {

    m_trayIconIndex++;

    if (m_trayIconIndex == 4) {
        m_trayAnimationTimer->stop();
        m_trayIconIndex = -1;
        m_trayAnimationTimer->start(2000);
        m_trayIcon->setIcon(*m_trayIcons[0]);
    }
    else
    {
        m_trayIcon->setIcon(*m_trayIcons[m_trayIconIndex]);
        if (m_trayIconIndex == 0)
            m_trayAnimationTimer->start(250);
    }
}

void QtFacebook::exitMenuAction() {
    exit(0);
}

void QtFacebook::testQueryConsole() {
    if (m_testConsole == 0)
        m_testConsole = new TestQueryConsole(m_userInfo);
    else if (m_testConsole->isMinimized())
        m_testConsole->showNormal();

    m_testConsole->show();

}

/************** Notifications ************************/
void QtFacebook::receivedNewNotifications(int numNew) {

    if (numNew != 0) {
        showNotifications(numNew);
        if (!m_trayAnimationTimer->isActive())
            m_trayAnimationTimer->start(250);
    }
    else
    {
        m_trayAnimationTimer->stop();
        m_trayIcon->setIcon(*m_trayIcons[0]);
        m_ackNotificationsMenuAction->setEnabled(false);
        m_notificationCountMenuAction->setText("You have 0 new notifications");
        m_notificationCountMenuAction->setEnabled(false);
        m_trayIcon->setToolTip("You have 0 new notifications");

    }



}



void QtFacebook::ackNewNotifications() {

    API::Factory *factory = new API::Factory(m_userInfo);
    API::Method *method = factory->createMethod("notifications.markRead");
    QString ids;
    while (!m_notificationList->empty()) {
        DATA::Notification *n = m_notificationList->takeFirst();
        ids.append(n->getNotificationId() + ",");
    }
    ids.chop(1);

    connect(factory, SIGNAL(apiNotificationsMarkRead(API::Notifications::MarkRead*)),
            this, SLOT(apiNotificationsMarkRead(API::Notifications::MarkRead*)));
    method->setArgument("notification_ids",ids);
    method->execute();

    m_trayAnimationTimer->stop();
    m_trayIcon->setIcon(*m_trayIcons[0]);
    m_ackNotificationsMenuAction->setEnabled(false);
    m_notificationCountMenuAction->setText("You have 0 new notifications");
    m_notificationCountMenuAction->setEnabled(false);
}

void QtFacebook::apiNotificationsMarkRead(API::Notifications::MarkRead *method) {

    qDebug() << "apiNotificationsMarkRead(): " << method->successful();

}

void QtFacebook::viewNewNotifications() {
    viewNotifications(GUI::Notifications::ListView::NEW);
}

void QtFacebook::viewAllNotifications() {
    viewNotifications(GUI::Notifications::ListView::ALL);
}

void QtFacebook::viewRecentNotifications() {
    viewNotifications(GUI::Notifications::ListView::RECENT);
}

void QtFacebook::viewNotifications(GUI::Notifications::ListView::mode m) {
    if (m_notificationListView == 0) {
        m_notificationListView = new GUI::Notifications::ListView(m_userInfo);
    } else if (m_notificationListView != 0 && m_notificationListView->isMinimized()) {
        m_notificationListView->showNormal();
        m_notificationListView->restoreWindow();
    }  else {
        //m_notificationListView->restoreWindow();
        m_notificationListView->activateWindow();
    }

    m_notificationListView->reload(m);

    m_notificationListView->show();
    m_notificationListView->raise();
}
