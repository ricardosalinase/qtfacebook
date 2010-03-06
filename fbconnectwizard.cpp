#include "fbconnectwizard.h"
#include "webview.h"
#include "util/cookiejar.h"
#include "util/OurUserInfo.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>

FBConnectWizard::FBConnectWizard(QString appName, bool firstTime, QWidget *parent) :
        QWizard(parent),
        m_appName(appName),
        m_firstTime(firstTime)
{
    setWizardStyle ( QWizard::ModernStyle );
    setOption ( QWizard::NoBackButtonOnStartPage, true);
    setMinimumSize(600,490);
    setPage(Page_Intro, createIntroPage());
    setPage(Page_Connect, createConnectPage());
    setPage(Page_Error, createErrorPage());
    setPage(Page_Conclusion, createConclusionPage());
}


QWizardPage* FBConnectWizard::createIntroPage() {

    QWizardPage *qwp = new QWizardPage();
    qwp->setTitle("Welcome!");
    QLabel *l;
    if (m_firstTime)
        l = new QLabel("Since this is your first time using " + m_appName
                       + ", you'll need to log into Facebook and authorize "
                       "this application to access your account.<br><br> "
                       "Facebook's terms of service require that this be done "
                       "through their Facebook Connect API and neither your email "
                       " address or password are saved by this application.<br><br>"
                       " Click 'Next' below to get started!");
    else
        l = new QLabel("It would appear that the session provided by Facebook through "
                       "their Facebook Connect API has become invalid (unfortunately "
                       "this happens occasionally). In order to continue using this"
                       "application you must log into Facebook again.<br><br> "
                       "Click 'Next' below to log back into Facebook via Facebook Connect");

    l->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(l);
    qwp->setLayout(layout);
    QPixmap pm;
    pm.load(":/uiImages/signGuy.jpg");

    qwp->setPixmap(QWizard::WatermarkPixmap, pm);
    return qwp;


}

QWizardPage* FBConnectWizard::createConnectPage() {
    ConnectPage *cp = new ConnectPage();

    connect(cp, SIGNAL(userAuthenticated()),
            this, SIGNAL(userHasAuthenticated()));

    return cp;

}


QWizardPage* FBConnectWizard::createErrorPage() {
    ErrorPage *ep = new ErrorPage();
    return ep;
}

QWizardPage* FBConnectWizard::createConclusionPage() {
    ConclusionPage *cp = new ConclusionPage();
    return cp;
}


ConnectPage::ConnectPage(QWidget *parent) :
        QWizardPage(parent),
        m_isComplete(false),
        m_gotAuth(false)
{
    m_view = new WebView(this);

    m_facebookUrl = "http://www.facebook.com/login.php?api_key=" +
            UTIL::OurUserInfo::getInstance()->getApiKey() +
             + "&connect_display=popup&v=1.0&"
             "next=http://www.facebook.com/connect/login_success.html"
             "&cancel_url=http://www.facebook.com/connect/login_failure.html"
             "&fbconnect=true&return_session=true&skipcookie=true"
             "&req_perms=read_stream,publish_stream,offline_access,read_mailbox,xmpp_login";

    connect(m_view, SIGNAL(authReceived()),
            this, SLOT(gotAuth()));
    connect(m_view, SIGNAL(authFailed()),
            this, SLOT(gotFailed()));

    UTIL::CookieJar *cj = new UTIL::CookieJar();

    QWebPage *wp = m_view->page();
    wp->networkAccessManager()->setCookieJar(cj);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_view);
    setLayout(layout);

    // This is not necessary, but it's a nice trick to get
    // the facebook connect page to automatically show the
    // email/pass fields when the page is reloaded by the
    // QWizard calling initializePage()
    initializePage();

}

void ConnectPage::initializePage() {
    QUrl url(m_facebookUrl);

    m_view->load(url);
}


void ConnectPage::gotAuth() {

    // Make sure that all the needed permissions were granted
    if (!m_view->permissionsGranted())
    {
        // nextId() will send us to the fail page
        initializePage();
        m_isComplete = true;
        emit completeChanged();
    }
    else
    {
        initializePage();

        UTIL::OurUserInfo::getInstance()->setSessionKey(m_view->getSessionKey());
        UTIL::OurUserInfo::getInstance()->setSecret(m_view->getSecret());
        UTIL::OurUserInfo::getInstance()->setUID(m_view->getUID());

        emit userAuthenticated();

        m_gotAuth = true;
        m_isComplete = true;
        emit completeChanged();
    }

    // If anyone else knows of a better way to do this ... let me know
    // It does seem to work fine though.
    ((QPushButton*)this->wizard()->button(QWizard::NextButton))->click();
}

void ConnectPage::gotFailed() {
    qDebug("Ruht Row ... something went wrong");

    initializePage();
    m_isComplete = true;
    emit completeChanged();

    // If anyone else knows of a better way to do this ... let me know
    // It does seem to work fine though.
    ((QPushButton*)this->wizard()->button(QWizard::NextButton))->click();
}

bool ConnectPage::isComplete() const {
    return m_isComplete;
}

bool ConnectPage::hasCompletedAuth() {
    return m_gotAuth;
}

int ConnectPage::nextId() const {
    if (m_view->permissionsGranted())
        return FBConnectWizard::Page_Conclusion;
    else
        return FBConnectWizard::Page_Error;
}

ErrorPage::ErrorPage(QWidget *parent) :
        QWizardPage(parent)
{
    QPixmap pm;
    pm.load(":/uiImages/thumbsDown3.jpg");
    setPixmap(QWizard::WatermarkPixmap, pm);
    setTitle("Ruh Roh!");

    QLabel *l = new QLabel("Unfortunately your Facebook Connect session didn't go as planned.<br><br>"
                "Please go back and try again. Please note that this application requires read,"
                " publish, read_mailbox, and offline access to function.");

    l->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(l);
    setLayout(layout);
}

bool ErrorPage::isComplete() const {
    return false;
}

ConclusionPage::ConclusionPage(QWidget *parent) :
        QWizardPage(parent)
{
    QPixmap pm;
    pm.load(":/uiImages/thumbsUp3.jpg");
    setPixmap(QWizard::WatermarkPixmap, pm);

    setTitle("Authorized!");


    QLabel *l = new QLabel("All set! This application can now access your facebook page.<br><br>"
                           "Click 'Finish' below to start the application.");
    l->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(l);
    setLayout(layout);

    setFinalPage(true);
}

int ConclusionPage::nextId() const {
    return -1;
}
