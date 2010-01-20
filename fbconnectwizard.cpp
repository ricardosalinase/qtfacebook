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
    setPage(Page_Intro, createIntroPage());
    setPage(Page_Connect, (QWizardPage *)new ConnectPage(m_apiKey));
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

ConnectPage::ConnectPage(QString apiKey, QWidget *parent) :
        QWizardPage(parent),
        m_apiKey(apiKey),
        m_isComplete(false),
        m_gotAuth(false)
{
    m_view = new WebView(this);

    m_facebookUrl = "http://www.facebook.com/login.php?api_key=" + m_apiKey
             + "&connect_display=popup&v=1.0&"
             "next=http://www.facebook.com/connect/login_success.html"
             "&cancel_url=http://www.facebook.com/connect/login_failure.html"
             "&fbconnect=true&return_session=true&session_key_only=true"
             "&req_perms=read_stream,publish_stream,offline_access";


    connect(m_view, SIGNAL(authReceived()),
            this, SLOT(gotAuth()));
    connect(m_view, SIGNAL(authFailed()),
            this, SLOT(gotFailed()));

    CookieJar *cj = new CookieJar();

    QWebPage *wp = m_view->page();
    wp->networkAccessManager()->setCookieJar(cj);
    //QLabel *l = new QLabel("(Click '<font color=blue>Log in with Facebook</font>' if you already have an account)");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_view);
    //layout->addWidget(l);
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

int ConnectPage::nextId() const {
    if (m_gotAuth)
        return FBConnectWizard::Page_Conclusion;
    else
        return FBConnectWizard::Page_Connect;
}

void ConnectPage::gotAuth() {

    // Make sure that all the needed permissions were granted
    if (!m_view->permissionsGranted())
    {
        // Display the thumbs down and explain why
        QString html ="<body><html>Thumbs Down!<br><br>" ;

        if (!m_view->hasReadPermission())
            html += "- You need to grant read permission<br>";
        if (!m_view->hasPublishPermission())
            html += "- You need to grant publish permission<br>";
        if (!m_view->hasOfflineAccessPermission())
            html += "- You need to grant offline access permission<br>";



        html += "<br><a href=\"" + m_facebookUrl + "\">Restart Facebook Connect</a></html></body>";

        m_view->setHtml(html);
        m_isComplete = true;
        emit completeChanged();

    }
    else
    {
        // Display the thumbs up

        m_view->setHtml("Thumbs up!");

        // Grab the info we need from the view and pass it to the
        // QWizard via a signal

        qDebug() << "Session Key: " << m_view->getSessionKey() << "\nuid: "
                << m_view->getUID() << "\nSecret: " << m_view->getSecret();
        if (m_view->permissionsGranted())
            qDebug("And our permissions were granted!");

        m_gotAuth = true;
        m_isComplete = true;
        emit completeChanged();

    }

}

void ConnectPage::gotFailed() {
    qDebug("Ruht Row ... something went wrong");


    //close();
}

bool ConnectPage::isComplete() const {
    return m_isComplete;
}

bool ConnectPage::hasCompletedAuth() {
    return m_gotAuth;
}
