#include <QDebug>

#include "qtfacebook.h"
#include "fbconnectwizard.h"


QtFacebook::QtFacebook(QWidget *parent) :
    QWidget(parent),
    m_userInfo(0)
{

    setVisible(false);

    FBConnectWizard *fbc = new FBConnectWizard("61cecf6f7ee5528d294e1d6bf675f424", "qtFacebook");

    connect(fbc, SIGNAL(userAuthenticated(UserInfo*)),
            this, SLOT(FbWizardComplete(UserInfo*)));


    fbc->show();



}

void QtFacebook::FbWizardComplete(UserInfo *info) {
    m_userInfo = info;

    qDebug() << "Session Key: " << info->getSessionKey() <<
            "Secret: " << info->getSecret() <<
            "UID" << info->getUID();

    // Start main application
}

