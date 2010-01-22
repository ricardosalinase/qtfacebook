#include <QDebug>
#include <QVBoxLayout>

#include "qtfacebook.h"
#include "fbconnectwizard.h"

QtFacebook::QtFacebook(QWidget *parent) :
    QWidget(parent),
    m_userInfo(0)
{

    // load session_key, uid, and secret


    // If we don't have those, launch the connector
    FBConnectWizard *fbc = new FBConnectWizard("61cecf6f7ee5528d294e1d6bf675f424", "qtFacebook");

    connect(fbc, SIGNAL(userHasAuthenticated(UserInfo*)),
            this, SLOT(saveUserInfo(UserInfo*)));

    connect(fbc, SIGNAL(accepted()),
            this, SLOT(fbWizardComplete()));
    connect(fbc, SIGNAL(rejected()),
            this, SLOT(fbWizardCanceled()));


    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(fbc);
    setLayout(layout);

    // Else just launch the main app
    // fbWizardComplete();


}

void QtFacebook::saveUserInfo(UserInfo *info) {
    m_userInfo = info;

    qDebug() << "Session Key: " << info->getSessionKey() <<
            "Secret: " << info->getSecret() <<
            "UID" << info->getUID();



}

void QtFacebook::fbWizardComplete() {
    setVisible(false);

    // Start main application



}

void QtFacebook::fbWizardCanceled() {
    close();
}
