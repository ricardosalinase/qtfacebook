#include <QDebug>
#include <QSettings>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QNetworkDiskCache>
#include "notifications_listview.h"



namespace GUI {
namespace Notifications {

 ListView::ListView(UserInfo *userInfo, QWidget *parent) :
            QDialog(parent),
            m_userInfo(userInfo)
{
     m_factory = new API::Factory(userInfo);

     connect(m_factory, SIGNAL(apiNotificationsGetList(API::Notifications::GetList*)),
             this, SLOT(apiNotificationsGetList(API::Notifications::GetList*)));


     QScrollArea *sa = new QScrollArea();
     sa->setWidgetResizable(true);
     m_nContainer = new QWidget();
     m_nContainer->resize(200,600);
     m_nContainer->setStyleSheet("background: white");
     QVBoxLayout *vbl = new QVBoxLayout();
     m_nContainer->setLayout(vbl);
     sa->setWidget(m_nContainer);
     sa->setStyleSheet("background: #e5e5e5");
    QVBoxLayout *mainLayout = new QVBoxLayout();

    //m_webView = new QWebView();
    //QWebPage *wp = m_webView->page();
    //wp->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    //connect(wp, SIGNAL(linkClicked(QUrl)),
    //        this, SLOT(navigate(QUrl)));


    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(sa);


    mainLayout->addLayout(bottomLayout);


    setLayout(mainLayout);


    setStyleSheet("background: #526ea6");
    restoreWindow();

    getNotifications(true);


}



void ListView::closeEvent(QCloseEvent *event) {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    settings.setValue("notificationsListView", saveGeometry());
    settings.endGroup();
    event->accept();
}

void ListView::restoreWindow() {
    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    QByteArray g = settings.value("notificationsListView").toByteArray();
    settings.endGroup();

    if (g.size() != 0)
    {
        restoreGeometry(g);
    }
    else
    {
        resize(200,600);
        move(QCursor::pos());
    }

}

void ListView::navigate(QUrl url) {
    qDebug() << "Url: " + url.toString();
}

void ListView::getNotifications(bool getAll) {

    qDebug() << "getNotifications()";

    API::Method *method = m_factory->createMethod("notifications.getList");

    if (getAll)
        method->setArgument("include_read","true");

    bool rc = method->execute();
    if (!rc)
        qDebug() << method->getErrorStr();
}

void ListView::apiNotificationsGetList(API::Notifications::GetList *method) {

    qDebug() << "apiNotificationsGetList()";


    m_appInfoMap = method->getAppInfo();
    m_nList = method->getNotifications();

    method->deleteLater();

    getPixmaps();
}

void ListView::getPixmaps() {

    connect(this, SIGNAL(gotAllPixmaps()),
            this, SLOT(displayResults()));

    QNetworkAccessManager *m_nam = new QNetworkAccessManager();
    QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPixmap(QNetworkReply*)));
    QNetworkDiskCache *diskCache = new QNetworkDiskCache();
    QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    diskCache->setCacheDirectory(location);
    m_nam->setCache(diskCache);


    QNetworkReply *reply;
    QMapIterator<QString, API::Notifications::AppInfo*> i(m_appInfoMap);
    while (i.hasNext()) {
        i.next();
        QUrl url(i.value()->getIconUrl());
        reply = m_nam->get(QNetworkRequest(url));
        m_tmpMap.insert(reply, i.key());
    }
}

void ListView::gotPixmap(QNetworkReply *reply) {

    qDebug() << "Got Pixmap reply; reply: " << reply;

    if (reply->error() == QNetworkReply::NoError)
    {
        QString aid = m_tmpMap.take(reply);
        API::Notifications::AppInfo *a = m_appInfoMap.value(aid);
        QPixmap *p = new QPixmap();
        p->loadFromData(reply->readAll());
        a->setIconPixmap(p);

    } else {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }

    reply->deleteLater();

    if (m_tmpMap.size() == 0)
        emit gotAllPixmaps();
}

void ListView::displayResults()
{
    qDebug() << "nList.size(): " << m_nList.size();


    Widget *nWidget;


    while (!m_nList.empty())
    {
        API::Notifications::Notification n = m_nList.takeFirst();
        API::Notifications::AppInfo *ai = m_appInfoMap.value(n.getAppId());
        nWidget = new Widget(n.getTitleHtml(), ai->getIconPixmap());
        m_nContainer->layout()->addWidget(nWidget);
    }









}


} // namespace Notifications
} // namespace GUI
