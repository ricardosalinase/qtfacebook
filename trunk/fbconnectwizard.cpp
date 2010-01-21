#include "fbconnectwizard.h"
#include "webview.h"
#include "cookiejar.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>

FBConnectWizard::FBConnectWizard(QString apiKey, QString appName, bool firstTime) :
        QWizard(),
        m_apiKey(apiKey),
        m_appName(appName),
        m_firstTime(firstTime)
{
    setMinimumSize(600,490);
    setPage(Page_Intro, createIntroPage());
    ConnectPage *cp = new ConnectPage(m_apiKey);

    connect(cp, SIGNAL(userAuthenticated(UserInfo*)),
            this, SIGNAL(userHasAuthenticated(UserInfo*)));




    setPage(Page_Connect, cp);
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
    pm.load("./uiImages/qtFacebookWizardSide.jpg");


    qwp->setPixmap(QWizard::WatermarkPixmap, pm);
    return qwp;


}

QWizardPage* FBConnectWizard::createConclusionPage() {
    QWizardPage *qwp = new QWizardPage();
    QPixmap pm;
    pm.load("./uiImages/qtFacebookWizardSide.jpg");
    qwp->setPixmap(QWizard::WatermarkPixmap, pm);

    qwp->setTitle("Authorized!");


    QLabel *l = new QLabel("All set! " + m_appName + " can now access your facebook page.<br><br>"
                           "Click 'Finish' below to start the application.");
    l->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(l);
    qwp->setLayout(layout);

    return qwp;
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

    m_thumbsUp = new QLabel();
    QPixmap tu("./uiImages/thumbsUp.jpg");
    m_thumbsUp->setPixmap(tu);
    m_thumbsUp->setVisible(false);

    m_thumbsDown = new QLabel();
    QPixmap td("./uiImages/thumbsDown.jpg");
    m_thumbsDown->setPixmap(td);
    m_thumbsDown->setVisible(false);

    connect(m_view, SIGNAL(authReceived()),
            this, SLOT(gotAuth()));
    connect(m_view, SIGNAL(authFailed()),
            this, SLOT(gotFailed()));

    CookieJar *cj = new CookieJar();

    QWebPage *wp = m_view->page();
    wp->networkAccessManager()->setCookieJar(cj);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_view);
    layout->addWidget(m_thumbsUp);
    layout->addWidget(m_thumbsDown);
    setLayout(layout);

    // This is not necessary, but it's a nice trick to get
    // the facebook connect page to automatically show the
    // email/pass fields when the page is reloaded by the
    // QWizard calling initializePage()
    initializePage();

}

void ConnectPage::initializePage() {

    m_thumbsUp->setVisible(false);
    m_thumbsDown->setVisible(false);
    m_view->setVisible(true);
    QUrl url(m_facebookUrl);

    m_view->load(url);
}


void ConnectPage::gotAuth() {

    // Make sure that all the needed permissions were granted
    if (!m_view->permissionsGranted())
    {
        // Display the thumbs down and explain why
        m_view->setVisible(false);
        m_thumbsDown->setVisible(true);

        QString html ="<body><html>Thumbs Down!<br><br>" ;

        if (!m_view->hasReadPermission())
            html += "- You need to grant read permission<br>";
        if (!m_view->hasPublishPermission())
            html += "- You need to grant publish permission<br>";
        if (!m_view->hasOfflineAccessPermission())
            html += "- You need to grant offline access permission<br>";

        html += "<br><a href=\"" + m_facebookUrl + "\">Restart Facebook Connect</a></html></body>";

        m_view->setHtml(html);


    }
    else
    {
        // Display the thumbs up
        m_view->setVisible(false);
        m_thumbsUp->setVisible(true);


        //m_view->setHtml("Thumbs up!");

        // Grab the info we need from the view and pass it to the
        // QWizard via a signal

        //qDebug() << "Session Key: " << m_view->getSessionKey() << "\nuid: "
        //        << m_view->getUID() << "\nSecret: " << m_view->getSecret();
        //if (m_view->permissionsGranted())
        //    qDebug("And our permissions were granted!");

        UserInfo *i = new UserInfo(m_view->getSessionKey(),m_view->getSecret(),m_view->getUID());
        emit userAuthenticated(i);

        m_gotAuth = true;
        m_isComplete = true;
        emit completeChanged();

    }

}

void ConnectPage::gotFailed() {
    qDebug("Ruht Row ... something went wrong");

    // Display the thumbs down and ask to restart FBC
    m_view->setVisible(false);
    m_thumbsDown->setVisible(true);

    //html += "It would appear something went horribly Awry";

    //html += "<br><a href=\"" + m_facebookUrl + "\">Restart Facebook Connect</a></html></body>";
    //m_view->setHtml(html);

}

bool ConnectPage::isComplete() const {
    return m_isComplete;
}

bool ConnectPage::hasCompletedAuth() {
    return m_gotAuth;
}
