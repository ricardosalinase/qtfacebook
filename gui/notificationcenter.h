#ifndef NOTIFICATIONCENTER_H
#define NOTIFICATIONCENTER_H

#include <QWidget>
#include <QEvent>
#include <QScrollArea>
#include <QList>
#include <QMap>
#include <QUrl>
#include <QEvent>
#include <QPair>
#include <QNetworkAccessManager>
#include <QWebView>


#include "api/factory.h"
#include "notificationcenterwidget.h"
#include "util/fbuserpiccache.h"
#include "gui/streampostwidget.h"
#include "data/FbPageInfo.h"
#include "gui/FbPhotoViewWidget.h"
#include "gui/FbAlbumViewWidget.h"


namespace GUI {

/*!
 * The NotificationCenter is the engine that runs the applicaiton. It includes the notificaiton window and
 * internally there is a second thread that checks for all new data being directed at the user. Navigation is
 * performed by sending signals to the other displays (including the tray icon).
 */

class NotificationCenter : public QWidget
{
Q_OBJECT
public:

    explicit NotificationCenter(QWidget *parent = 0);

signals:
    void receivedNewNotifications(int numNew);
    void receivedNewStreamPosts(int numNew);
    void acknowledgedNotification(GUI::NotificationCenterItem::ItemType, QString nId);

public slots:
    void showYourself();
    void newNotifications(QList<DATA::Notification*> *nList);
    void newNotification(DATA::Notification *n);
    void linkActivated(QString url);
    void notificationAcknowledged(NotificationCenterItem::ItemType, QString nId);



private slots:
    void receiveIconPixmap(QNetworkReply *reply);
    void apiFqlGetNewNotifications(API::FQL::GetNewNotifications*);
    void notificationGetFailed(API::FQL::GetNewNotifications *method);
    void apiFqlGetStreamPosts(API::FQL::GetStreamPosts*);
    void getStreamPostsFailed(API::FQL::GetStreamPosts*);
    void notificationsMarkedAsRead(API::Notifications::MarkRead*);
    void notificationsMarkedAsReadFailed(API::Notifications::MarkRead*);
    void newStreamPosts(QList<DATA::FbStreamPost *> *pList);
    void streamPostClosed(GUI::StreamPostWidget *, QString);
    void contentClicked(QString url);
    void photoViewClosed(GUI::FbPhotoViewWidget *);
    void albumViewClosed(GUI::FbAlbumViewWidget *);

protected:
    void closeEvent ( QCloseEvent * event );


private:
    void restoreWindow();
    void getPixmap(QLabel *, DATA::AppInfo& ai);
    void getPixmap(QLabel *, DATA::FbUserInfo& fbu);
    void getPixmap(QLabel *, DATA::FbPageInfo& fbp);
    void getInitialNotifications();
    QNetworkAccessManager *m_nam;
    QMap<QNetworkReply *, QPair<QString, QLabel *> > m_tmpMap;
    QMap<QString, QPixmap> m_iconPixmapCache;
    bool m_showHiddenNotifications;
    bool m_showHiddenStreamPosts;
    API::Factory *m_factory;
    QList<DATA::Notification*> *m_notificationList;
    QMap<QString, DATA::FbStreamPost *> m_streamPosts;
    QMap<QString, GUI::StreamPostWidget *> m_openPosts;
    bool m_firstReception;


    // UI componenets
    QScrollArea *m_scrollArea;
    QWidget *m_nContainer;
    QMap<QString, GUI::NotificationCenterWidget *> m_notifications;
    QWebView *m_webView;

};

} // namespace GUI

#endif // NOTIFICATIONCENTER_H
