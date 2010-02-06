#ifndef NOTIFICATIONCENTER_H
#define NOTIFICATIONCENTER_H

#include <QWidget>
#include <QEvent>
#include <QScrollArea>
#include <QList>
#include <QMap>
#include <QUrl>
#include <QEvent>


#include "userinfo.h"
#include "api/factory.h"
#include "notificationcenterwidget.h"
#include "notificationcheck.h"


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

    NotificationCenter(UserInfo *userInfo, QWidget *parent = 0);

signals:
    void receivedNewNotifications(int numNew);
    void acknowledgedNotification(QString nId);

public slots:
    void showYourself();
    void newNotifications(QList<DATA::Notification*> *nList, QMap<QString, DATA::AppInfo *> *aMap);
    void linkActivated(QString url);
    void notificationAcknowledged(QString nId);
    void notificationsMarkedAsRead(API::Notifications::MarkRead*);
private slots:
    void receiveIconPixmap(QNetworkReply *reply);
    //void notificationAcknowledged(QString nId);
    //void notificationsMarkedAsRead(API::Notifications::MarkRead*);

protected:
    void closeEvent ( QCloseEvent * event );
    //void changeEvent(QEvent *event);

private:
    void navigate(QUrl url);
    void restoreWindow();
    void getPixmap(GUI::AppInfoLabel *ai);

    NotificationCheck *m_notificationCheck;
    UserInfo *m_userInfo;
    QMap<QNetworkReply *, GUI::AppInfoLabel *> m_tmpMap;
    QMap<QString, QPixmap *> m_iconPixmapCache;
    bool m_showHiddenNotifications;

    // UI componenets
    QScrollArea *m_scrollArea;
    QWidget *m_nContainer;

};

} // namespace GUI

#endif // NOTIFICATIONCENTER_H
