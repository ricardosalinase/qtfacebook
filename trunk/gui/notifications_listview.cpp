#include <QDebug>
#include <QSettings>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QNetworkDiskCache>
#include <QPixmapCache>
#include "notifications_listview.h"



namespace GUI {
namespace Notifications {

 ListView::ListView(UserInfo *userInfo, QWidget *parent) :
            QWidget(parent),
            m_userInfo(userInfo)
{
    m_factory = new API::Factory(userInfo);

    connect(m_factory, SIGNAL(apiNotificationsGetList(API::Notifications::GetList*)),
             this, SLOT(apiNotificationsGetList(API::Notifications::GetList*)));
    connect(this, SIGNAL(gotAllPixmaps()),
            this, SLOT(displayResults()));

    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_nContainer = new QWidget();
    m_nContainer->resize(200,600);
    m_nContainer->setStyleSheet("background: white");
    QVBoxLayout *vbl = new QVBoxLayout();
    m_nContainer->setLayout(vbl);
    m_scrollArea->setWidget(m_nContainer);
    m_scrollArea->setStyleSheet("background: #e5e5e5");
    QVBoxLayout *mainLayout = new QVBoxLayout();

    //m_webView = new QWebView();
    //QWebPage *wp = m_webView->page();
    //wp->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    //connect(wp, SIGNAL(linkClicked(QUrl)),
    //        this, SLOT(navigate(QUrl)));


    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(m_scrollArea);


    mainLayout->addLayout(bottomLayout);


    setLayout(mainLayout);


    setStyleSheet("background: #526ea6");
    restoreWindow();

    //getNotifications(m);


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

void ListView::reload(mode m) {

    qDebug() << "reload()";

    API::Method *method = m_factory->createMethod("notifications.getList");

    if (m == ALL || RECENT)
        method->setArgument("include_read","true");

    if (m == RECENT) {
        time_t current = time(0);
        time_t fromTime = current -= (60 * 60 * 24);
        QString startTime = QString::number(fromTime);
        method->setArgument("start_time",startTime);
    }




    bool rc = method->execute();
    if (!rc)
        qDebug() << method->getErrorStr();
}

void ListView::apiNotificationsGetList(API::Notifications::GetList *method) {

    qDebug() << "apiNotificationsGetList()";


    m_appInfoMap = method->getAppInfoMap();
    m_notificationList = method->getNotificationList();

    method->deleteLater();

    getPixmaps();
}

void ListView::getPixmaps() {

    // Send off network requests to download the pixmaps.
    // TODO: The QPixmapCache is NOT thread-safe. Need to implement QThreadStorage


    // TODO: Also maybe do partial reads rather than waiting for the entire reply to come back?



    QNetworkAccessManager *m_nam = new QNetworkAccessManager();
    QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPixmap(QNetworkReply*)));
    QNetworkDiskCache *diskCache = new QNetworkDiskCache();
    QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    diskCache->setCacheDirectory(location);
    m_nam->setCache(diskCache);


    QNetworkReply *reply;

    bool sentRequest = false;
    QPixmap *pixmap;

    QMap<QString,API::Notifications::AppInfo* >::const_iterator i = m_appInfoMap->constBegin();
    while (i != m_appInfoMap->constEnd()) {
        API::Notifications::AppInfo *ai = i.value();
        //if (!QPixmapCache::find(ai->getAppId(), pixmap )) {
            sentRequest = true;
            QUrl url(ai->getIconUrl());
            reply = m_nam->get(QNetworkRequest(url));
            m_tmpMap.insert(reply, i.key());
            ++i;
        //}
        //else
        //    ai->setIconPixmap(pixmap);
    }

    if (!sentRequest)
        emit gotAllPixmaps();


}

void ListView::gotPixmap(QNetworkReply *reply) {

    qDebug() << "Got Pixmap reply; reply: " << reply;

    if (reply->error() == QNetworkReply::NoError)
    {
        QString aid = m_tmpMap.take(reply);
        API::Notifications::AppInfo *a = m_appInfoMap->value(aid);
        QPixmap *p = new QPixmap();
        p->loadFromData(reply->readAll());
        a->setIconPixmap(p);
        //QPixmapCache::insert(a->getAppId(),*p);

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
    qDebug() << "nList.size(): " << m_notificationList->size();

    // Clear current display
    Widget *child;
    while ((child = (Widget*) m_nContainer->layout()->takeAt(0)) != 0)
        delete child;

    m_nContainer = new QWidget();
    m_nContainer->resize(200,600);
    m_nContainer->setStyleSheet("background: white");
    QVBoxLayout *vbl = new QVBoxLayout();
    m_nContainer->setLayout(vbl);

    m_scrollArea->setWidget(m_nContainer);

    Widget *nWidget;


    while (!m_notificationList->empty())
    {
        API::Notifications::Notification *n = m_notificationList->takeFirst();
        API::Notifications::AppInfo *ai = m_appInfoMap->value(n->getAppId());
        nWidget = new Widget(n->getTitleHtml(), ai->getIconPixmap());
        m_nContainer->layout()->addWidget(nWidget);
        delete n;
    }

    // Clear the AppInfoMap
    // TODO: Think about caching this data?
    QMap<QString, API::Notifications::AppInfo*>::const_iterator i = m_appInfoMap->constBegin();
    while (i != m_appInfoMap->constEnd()) {
        delete i.value();
        i++;
    }

    delete m_appInfoMap;
    delete m_notificationList;

}


} // namespace Notifications
} // namespace GUI
