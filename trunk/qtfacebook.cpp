#include <QDebug>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QDir>
#include <QCoreApplication>
#include <QSettings>

#include "qtfacebook.h"
#include "fbconnectwizard.h"
#include "testqueryconsole.h"

QtFacebook::QtFacebook(QWidget *parent) :
    QWidget(parent),
    m_userInfo(0),
    m_layout(0),
    m_wizard(0)
{

    // load session_key, uid, and secret
    bool hasInfo = loadUserInfo();

    m_layout = new QVBoxLayout();
    setLayout(m_layout);

    if (!hasInfo) {
        // If we don't have those, launch the connector
        m_wizard = new FBConnectWizard("61cecf6f7ee5528d294e1d6bf675f424", "qtFacebook");

        connect(m_wizard, SIGNAL(userHasAuthenticated(UserInfo*)),
                this, SLOT(saveUserInfo(UserInfo*)));

        connect(m_wizard, SIGNAL(accepted()),
                this, SLOT(fbWizardComplete()));
        connect(m_wizard, SIGNAL(rejected()),
                this, SLOT(fbWizardCanceled()));

        m_layout->addWidget(m_wizard);


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

    m_userInfo = new UserInfo(sKey, secret, uid);

    qDebug() << "Session Key: " << m_userInfo->getSessionKey() <<
            "Secret: " << m_userInfo->getSecret() <<
            "UID" << m_userInfo->getUID();


    return true;



}


void QtFacebook::fbWizardComplete() {

    m_layout->removeWidget(m_wizard);

    // Start main application
    TestQueryConsole *tqc = new TestQueryConsole();
    m_layout->addWidget(tqc);


}

void QtFacebook::fbWizardCanceled() {
    close();
}
