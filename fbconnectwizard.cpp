#include "fbconnectwizard.h"
#include "webview.h"
#include "cookiejar.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

FBConnectWizard::FBConnectWizard(QString apiKey, QString appName, bool firstTime) :
        QWizard(),
        m_apiKey(apiKey),
        m_appName(appName),
        m_firstTime(firstTime)
{
    setMinimumSize(600,450);
    addPage(createIntroPage());
    addPage(createConnectPage());

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

    return qwp;


}

QWizardPage* FBConnectWizard::createConnectPage() {
    QWizardPage *qwp = new QWizardPage();

    WebView *view = new WebView(this);
    m_view = view;

    connect(view, SIGNAL(authReceived()),
            this, SLOT(gotAuth()));
    connect(view, SIGNAL(authFailed()),
            this, SLOT(gotFailed()));

    CookieJar *cj = new CookieJar();

    QWebPage *wp = view->page();
    wp->networkAccessManager()->setCookieJar(cj);

    QUrl url("http://www.facebook.com/login.php?api_key=" + m_apiKey
             + "&connect_display=popup&v=1.0&"
             "next=http://www.facebook.com/connect/login_success.html"
             "&cancel_url=http://www.facebook.com/connect/login_failure.html"
             "&fbconnect=true&return_session=true&session_key_only=true"
             "&req_perms=read_stream,publish_stream,offline_access");

    view->load(url);

    QLabel *l = new QLabel("(Click on <font color=blue>Log in with Facebook</font> if you already have an account)");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addWidget(l);
    qwp->setLayout(layout);

    return qwp;
}

QWizardPage* FBConnectWizard::createConclusionPage() {
    QWizardPage *qwp = new QWizardPage();
    return qwp;
}

void FBConnectWizard::gotAuth() {
    qDebug() << "Session Key: " << m_view->getSessionKey() << "\nuid: "
            << m_view->getUID() << "\nSecret: " << m_view->getSecret();
    if (m_view->permissionsGranted())
        qDebug("And our permissions were granted!");

    //close();
}

void FBConnectWizard::gotFailed() {
    qDebug("Ruht Row ... something went wrong");
    //close();
}
