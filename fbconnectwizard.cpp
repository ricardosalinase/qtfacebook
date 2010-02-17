#include "fbconnectwizard.h"
#include "webview.h"
#include "util/cookiejar.h"
#include "util/facebooklogin.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>

FBConnectWizard::FBConnectWizard(UserInfo *userInfo, QString appName, bool firstTime) :
        QWizard(),
        m_appName(appName),
        m_firstTime(firstTime),
        m_userInfo(userInfo)
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


    IntroPage *ip = new IntroPage(m_userInfo, m_appName);
    return ip;

}

QWizardPage* FBConnectWizard::createConnectPage() {
    ConnectPage *cp = new ConnectPage(m_userInfo);

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

IntroPage::IntroPage(UserInfo *userInfo, QString appName, QWidget *parent) :
        QWizardPage(parent),
        m_gotLoggedIn(false),
        m_userInfo(userInfo)
{

    QPixmap pm;
    pm.load(":/uiImages/signGuy.jpg");
    setPixmap(QWizard::WatermarkPixmap, pm);



    setTitle("Welcome!");

    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *l;


    l = new QLabel("Since this is your first time using " + appName
                   + ", you'll need to log into Facebook and authorize "
                   "this application to access your page.");
    l->setWordWrap(true);
    layout->addWidget(l,0,Qt::AlignTop);

    QGridLayout *layout2 = new QGridLayout();
    QLabel *emailLabel = new QLabel("Email Address:");
    m_emailEdit = new QLineEdit();

    layout2->addWidget(emailLabel,0,0);
    layout2->addWidget(m_emailEdit,0,1);

    QLabel *pwLabel = new QLabel("Password:");
    m_pwEdit = new QLineEdit();
    m_pwEdit->setEchoMode(QLineEdit::Password);

    layout2->addWidget(pwLabel,1,0);
    layout2->addWidget(m_pwEdit,1,1);

    m_login = new QPushButton("Login");
    connect(m_login, SIGNAL(clicked()),
            this, SLOT(loginClicked()));

    layout2->addWidget(m_login,2,0,1,0,Qt::AlignRight);

    layout->insertSpacing(1,40);
    layout->addLayout(layout2);

    m_loginResult = new QLabel();
    m_loginResult->setWordWrap(true);
    layout->addWidget(m_loginResult);
    setLayout(layout);

}

void IntroPage::loginClicked() {

    //qDebug() << "**** LOGIN CLICKED? ****";

    m_login->setEnabled(false);
    m_loginResult->setText("");

    m_userInfo->setEmailAddy(m_emailEdit->text());
    m_userInfo->setPass(m_pwEdit->text());

    UTIL::FacebookLogin *fbl = new UTIL::FacebookLogin(m_userInfo, this);
    connect(fbl, SIGNAL(loginResults(bool)),
            this, SLOT(gotLoginResults(bool)));
    fbl->logIn();
}

void IntroPage::gotLoginResults(bool success) {

    if (success) {

        m_gotLoggedIn = true;

        m_loginResult->setText("Great! Now we just need to allow access to your page<br>"
                              "Click 'Next' to continue.");

        emit completeChanged();


    } else {

        m_loginResult->setText("Hmmm. That didn't seem to work out. Perhaps you entered"
                               " the wrong password?");

        m_login->setEnabled(true);


    }

}



bool IntroPage::isComplete() const {
    return m_gotLoggedIn;
}



ConnectPage::ConnectPage(UserInfo *userInfo, QWidget *parent) :
        QWizardPage(parent),
        m_isComplete(false),
        m_gotAuth(false),
        m_userInfo(userInfo)
{
    m_view = new WebView(this);

    m_facebookUrl = "http://www.facebook.com/login.php?api_key=" + m_userInfo->getApiKey()
             + "&connect_display=popup&v=1.0&"
             "next=http://www.facebook.com/connect/login_success.html"
             "&cancel_url=http://www.facebook.com/connect/login_failure.html"
             "&fbconnect=true&return_session=true" /* &session_key_only=true" */
             "&req_perms=read_stream,publish_stream,offline_access,read_mailbox,xmpp_login";

    connect(m_view, SIGNAL(authReceived()),
            this, SLOT(gotAuth()));
    connect(m_view, SIGNAL(authFailed()),
            this, SLOT(gotFailed()));

    QWebPage *wp = m_view->page();
    wp->networkAccessManager()->setCookieJar(userInfo->getCookieJar());
    userInfo->getCookieJar()->setParent(0);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_view);
    setLayout(layout);

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

        m_userInfo->setSessionKey(m_view->getSessionKey());
        m_userInfo->setSecret(m_view->getSecret());
        m_userInfo->setUID(m_view->getUID());


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



