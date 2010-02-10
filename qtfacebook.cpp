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
#include <QDateTime>

#include "qtfacebook.h"
#include "fbconnectwizard.h"
#include "testqueryconsole.h"
#include "api/factory.h"
#include "notificationcheck.h"
#include "util/facebooklogin.h"
#include "gui/facebooklogindialog.h"
#include "gui/loginfaileddialog.h"

#define API_KEY "61cecf6f7ee5528d294e1d6bf675f424"

QtFacebook::QtFacebook(QObject *parent) :
    QObject(parent),
    m_wizard(0),
    m_testConsole(0),
    m_notificationListView(0),
    m_notificationList(0),
    m_traySingleClicked(false),
    m_balloonMessageClicked(false),
    m_trayIconIndex(0),
    m_animatingTrayIcon(false),
    m_totalNotifications(0),
    m_standardNotifications(0),
    m_invalidLogin(0),
    m_loginDialog(0)
{

    QApplication::setQuitOnLastWindowClosed(false);
    m_userInfo = new UserInfo(API_KEY);
    m_loginDialog = new GUI::FacebookLoginDialog(m_userInfo);
    connect(m_loginDialog, SIGNAL(loginEntered()),
            this, SLOT(gotLoginInfo()) );
    connect(m_loginDialog, SIGNAL(loginCanceled()),
              this, SLOT(loginCanceled()));

    // load session_key, uid, and secret, email, pass, etc
    if (loadUserInfo()) {

        // TODO: Check to see if session is still valid via API

        if (m_userInfo->getPass().compare("") == 0) {

            m_loginDialog->show();

        } else {

           gotLoginInfo();

        }


    } else {

        m_wizard = new FBConnectWizard(m_userInfo, "qtFacebook");

        connect(m_wizard, SIGNAL(userHasAuthenticated()),
                this, SLOT(saveUserInfo()));

        connect(m_wizard, SIGNAL(accepted()),
                this, SLOT(fbWizardComplete()));
        connect(m_wizard, SIGNAL(rejected()),
                this, SLOT(fbWizardCanceled()));

        m_wizard->show();

    }

}

void QtFacebook::gotLoginInfo() {

    if (m_loginDialog != 0)
        m_loginDialog->close();

    UTIL::FacebookLogin *fbl = new UTIL::FacebookLogin(m_userInfo, this);
    connect(fbl, SIGNAL(loginResults(bool)),
            this, SLOT(gotLoginResults(bool)));
    fbl->logIn();
}

void QtFacebook::loginCanceled() {
    exit(0);
}

void QtFacebook::gotLoginResults(bool success) {

    if (success) {

        saveUserInfo();
        fbWizardComplete();

    } else {

        if (m_invalidLogin == 0) {
            m_invalidLogin = new GUI::LoginFailedDialog();
            connect(m_invalidLogin, SIGNAL(accepted()),
                    this, SLOT(loginFailedDialogClosed()));
        }
        m_invalidLogin->exec();
    }

}

void QtFacebook::loginFailedDialogClosed() {
    m_loginDialog->show();
}

void QtFacebook::saveUserInfo() {


    qDebug() << "Session Key: " << m_userInfo->getSessionKey() <<
            "Secret: " << m_userInfo->getSecret() <<
            "UID" << m_userInfo->getUID();

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("userInfo");
    settings.setValue("SessionKey",m_userInfo->getSessionKey());
    settings.setValue("UID", m_userInfo->getUID());
    settings.setValue("Secret", m_userInfo->getSecret());
    settings.setValue("Email", m_userInfo->getEmailAddy());
    settings.setValue("Pass", m_userInfo->getPass());
    settings.setValue("PostFormId", m_userInfo->getPostFormId());
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
    QString email(settings.value("Email","").toString());
    QString pass(settings.value("Pass","").toString());
    QString pfi(settings.value("PostFormId","").toString());
    settings.endGroup();

    if (sKey.compare("") == 0 ||
            uid.compare("") == 0 ||
            secret.compare("") == 0 ||
            email.compare("") == 0)
        return false;


    m_userInfo->setSessionKey(sKey);
    m_userInfo->setUID(uid);
    m_userInfo->setSecret(secret);
    m_userInfo->setEmailAddy(email);
    m_userInfo->setPass(pass);
    m_userInfo->setPostFormId(pfi);

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



    m_trayIcon = new QSystemTrayIcon(*m_trayIcons[0]);

    QMenu *menu = new QMenu();

    QAction *tqc = menu->addAction("Test Query Console");
    QAction *gbl = menu->addAction("Get Buddy List");
    menu->addSeparator();
    QAction *ncr = menu->addAction("Notification Center");
    menu->addSeparator();
    m_notificationCountMenuAction = menu->addAction("You have 0 new notifications");
    m_notificationCountMenuAction->setDisabled(true);

    QAction *act = menu->addAction("View Recent");
    m_viewAllNotificationsMenuAction = menu->addAction("View all");
    menu->addSeparator();
    QAction *exit = menu->addAction("Exit");

    connect(tqc, SIGNAL(triggered()),
            this, SLOT(testQueryConsole()));

    connect(exit, SIGNAL(triggered()),
            this, SLOT(exitMenuAction()));

    connect(m_viewAllNotificationsMenuAction, SIGNAL(triggered()),
            this, SLOT(viewAllNotifications()));
    connect(act, SIGNAL(triggered()),
            this, SLOT(viewRecentNotifications()));



    m_trayIcon->setContextMenu(menu);
    m_trayIcon->show();

    m_notificationCenter = new GUI::NotificationCenter(m_userInfo);
    connect(ncr, SIGNAL(triggered()),
            m_notificationCenter, SLOT(showYourself()));
    connect(m_notificationCenter, SIGNAL(receivedNewNotifications(int)),
            this, SLOT(receivedNewNotifications(int)));
    connect(m_notificationCenter, SIGNAL(acknowledgedNotification(QString)),
            this, SLOT(acknowledgedNotification(QString)));

    m_cometConnector = new CometConnector(m_userInfo);
    connect(m_cometConnector, SIGNAL(newNotification(DATA::Notification*,DATA::AppInfo*)),
            m_notificationCenter, SLOT(newNotification(DATA::Notification*,DATA::AppInfo*)),
            Qt::QueuedConnection);
    connect(m_cometConnector, SIGNAL(notificationAck(QString)),
            m_notificationCenter, SLOT(deactivateNotification(QString)),
            Qt::QueuedConnection);
    connect(m_cometConnector, SIGNAL(newChatMessage(DATA::ChatMessage*)),
            this, SLOT(newChatMessageReceived(DATA::ChatMessage*)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(newChatMessage(DATA::ChatMessage*)),
            m_cometConnector, SIGNAL(sendChatMessage(DATA::ChatMessage*)),
            Qt::QueuedConnection);
    connect(gbl, SIGNAL(triggered()),
            m_cometConnector, SIGNAL(getBuddyList()),
            Qt::QueuedConnection);
    connect(m_cometConnector, SIGNAL(newBuddyList(QList<DATA::Buddy*>*,QMap<QString,QString>*)),
            this, SLOT(gotBuddyList(QList<DATA::Buddy*>*,QMap<QString,QString>*)),
            Qt::QueuedConnection);


    m_cometConnector->start();


    if (!QSystemTrayIcon::supportsMessages())
        qDebug() << "Awwww ... bummer. It doesn't support messages";

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

    QTimer::singleShot(200, this, SLOT(updateIcon()));

    m_trayAnimationTimer = new QTimer();
    connect(m_trayAnimationTimer, SIGNAL(timeout()),
            this, SLOT(nextTrayIcon()));



    DATA::ChatMessage *cm = new DATA::ChatMessage();
    cm->setMsgId("12345");
    QDateTime qd = QDateTime::currentDateTime().toUTC();
    cm->setClientTime(QString::number(qd.toTime_t()) + QString::number(qd.time().msec()));
    cm->setToId("1082239928");
    cm->setText("Hey there Brian!");

    emit newChatMessage(cm);


}



void QtFacebook::updateIcon() {
    m_trayIcon->showMessage("Startup","You've started qtFacebook!",QSystemTrayIcon::Information);
}

void QtFacebook::trayActivated(QSystemTrayIcon::ActivationReason reason) {

    qDebug() << "trayActivated()";
    if (reason == QSystemTrayIcon::Trigger ) {
        if (m_notificationCenter->isVisible() && !m_notificationCenter->isMinimized())
            m_notificationCenter->close();
        else
            m_notificationCenter->showYourself();
    }
}


void QtFacebook::showNotifications(bool showBalloonMessage)
{
    QString s = "You have " + QString::number(m_totalNotifications) + " new notifications.";
    m_notificationCountMenuAction->setText(s);
    m_trayIcon->setToolTip(s);

    if (showBalloonMessage)
        m_trayIcon->showMessage("New Notifications", s,QSystemTrayIcon::Information, 15000);

    m_notificationCountMenuAction->setText("View "
                                           + QString::number(m_standardNotifications)
                                           + " new notifications");
    if (m_totalNotifications == 0) {
        m_trayAnimationTimer->stop();
        m_trayIcon->setIcon(*m_trayIcons[0]);
        m_notificationCountMenuAction->setEnabled(false);
    }
    else
    {
        if (!m_trayAnimationTimer->isActive())
            m_trayAnimationTimer->start(250);
        m_notificationCountMenuAction->setEnabled(true);
    }
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

    m_totalNotifications += numNew;
    m_standardNotifications += numNew;
    showNotifications(true);

}

void QtFacebook::acknowledgedNotification(QString /*nId*/) {

    m_totalNotifications--;
    m_standardNotifications--;
    showNotifications(false);

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

void QtFacebook::newChatMessageReceived(DATA::ChatMessage *c) {
    qDebug() << "From: " << c->getFromName() << " Msg: " << c->getText();
}

void QtFacebook::gotBuddyList(QList<DATA::Buddy*> *bList,QMap<QString,QString> *listInfo) {
    qDebug() << "Buddy List:";

    for (int i = 0; i < bList->size(); i++) {
        qDebug() << bList->at(i)->getFullName();
    }

}
