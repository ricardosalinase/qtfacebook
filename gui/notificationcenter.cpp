#include <QSettings>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QNetworkReply>

#include "notificationcenter.h"
#include "appinfolabel.h"
#include "notificationlabel.h"
#include "notificationwidget.h"


namespace GUI {

NotificationCenter::NotificationCenter(UserInfo *userInfo, QWidget *parent) :
    QWidget(parent),
    m_userInfo(userInfo),
    m_showHidden(false)
{
    m_scrollArea = new QScrollArea();
    m_scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 10px; }");
    m_scrollArea->setWidgetResizable(true);
    m_nContainer = new QWidget();
    m_nContainer->resize(200,600);
    m_nContainer->setStyleSheet("background: white;");
    QVBoxLayout *vbl = new QVBoxLayout();
    m_nContainer->setLayout(vbl);
    m_scrollArea->setWidget(m_nContainer);
    m_scrollArea->setStyleSheet("background: #e5e5e5");
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(m_scrollArea);


    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    setStyleSheet("background: #526ea6");

    m_notificationCheck = new NotificationCheck(m_userInfo,1);
    connect(m_notificationCheck, SIGNAL(newNotifications(QList<DATA::Notification*>*,QMap<QString,DATA::AppInfo*>*)),
            this, SLOT(newNotifications(QList<DATA::Notification*>*,QMap<QString,DATA::AppInfo*>*)),
            Qt::QueuedConnection);

    m_notificationCheck->start();


}

void NotificationCenter::showYourself() {

    if (!isVisible())
    {
        restoreWindow();
    }

    if (isMinimized())
        showNormal();
    else
        activateWindow();


    show();
    raise();


}

void NotificationCenter::restoreWindow() {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    QByteArray g = settings.value("NotificationCenter").toByteArray();
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

void NotificationCenter::navigate(QUrl url) {
    qDebug() << "Url: " + url.toString();
}

void NotificationCenter::newNotifications(QList<DATA::Notification *> *nList, QMap<QString, DATA::AppInfo *> *aMap) {

    qDebug() << "newNotifications(); nList: " << nList->size();

    // Notify Tray Icon
    emit receivedNewNotifications(nList->size());

    // If the list is empty and we still think there's new notificaions,
    // the user viewed them via facebook and they're no longer "unread"
    //if (nList->size() == 0 && our new list != 0 )
    // return
    //else
    // Display new Notifications
    /*m_nContainer = new QWidget();
    m_nContainer->resize(200,600);
    m_nContainer->setStyleSheet("background: white");
    QVBoxLayout *vbl = new QVBoxLayout();
    m_nContainer->setLayout(vbl);

    m_scrollArea->setWidget(m_nContainer);*/

    GUI::NotificationWidget *nWidget;

    while (!nList->empty())
    {
        GUI::NotificationLabel *n = new GUI::NotificationLabel(nList->takeFirst());
        GUI::AppInfoLabel *ai = new GUI::AppInfoLabel(new AppInfo(*(aMap->value(n->getNotification()->getAppId()))));
        getPixmap(ai);
        nWidget = new GUI::NotificationWidget(n, ai);
        connect(nWidget, SIGNAL(linkActivated(QString)),
                this, SLOT(linkActivated(QString)));
        ((QVBoxLayout*)m_nContainer->layout())->insertWidget(0,nWidget);
        nWidget->start();

    }


    AppInfo *tmp;
    QMap<QString, AppInfo *>::iterator i = aMap->begin();
     while (i != aMap->end()) {
             tmp = i.value();
             ++i;
             delete tmp;
     }

     delete nList;
     delete aMap;

    // If there's not enough to cause scrollbars, this will bunch them
    // at the top.
    ((QVBoxLayout *)m_nContainer->layout())->addStretch();

}

void NotificationCenter::getPixmap(GUI::AppInfoLabel *ai) {

    if (m_iconPixmapCache.contains(ai->getAppInfo()->getAppId()))
        ai->myIconPixmap(m_iconPixmapCache[ai->getAppInfo()->getAppId()], true);
    else {
        QNetworkAccessManager *m_nam = new QNetworkAccessManager();
        QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(receiveIconPixmap(QNetworkReply*)));

        QNetworkReply *reply;

        QUrl url(ai->getAppInfo()->getIconUrl());
        reply = m_nam->get(QNetworkRequest(url));
        m_tmpMap.insert(reply, ai);

    }

}

void NotificationCenter::receiveIconPixmap(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {

        GUI::AppInfoLabel *a = m_tmpMap.take(reply);
        QPixmap *p = new QPixmap();
        p->loadFromData(reply->readAll());
        a->myIconPixmap(p, true);
        m_iconPixmapCache.insert(a->getAppInfo()->getAppId(), p);

    } else {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }

}

void NotificationCenter::linkActivated(QString url) {
    qDebug() << url;
}

void NotificationCenter::closeEvent(QCloseEvent *event) {

    QSettings settings("qtFacebook","qtFacebook");
    settings.beginGroup("windowGeometry");
    settings.setValue("NotificationCenter", saveGeometry());
    settings.endGroup();
    event->accept();
}


} // namespace GUI
