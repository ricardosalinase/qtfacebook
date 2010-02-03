#ifndef NOTIFICATIONCENTER_H
#define NOTIFICATIONCENTER_H

#include <QWidget>
#include <QEvent>
#include <QScrollArea>
#include <QList>
#include <QMap>
#include <QUrl>


#include "userinfo.h"
#include "api/factory.h"
#include "notifications_widget.h"
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

public slots:
    void showYourself();
    void newNotifications(QList<DATA::Notification*> *nList, QMap<QString, DATA::AppInfo *> *aMap);
    void linkActivated(QString url);

private slots:
    void receiveIconPixmap(QNetworkReply *reply);

protected:
    void closeEvent ( QCloseEvent * event );

private:
    void navigate(QUrl url);
    void restoreWindow();
    void getPixmap(GUI::AppInfoLabel *ai);

    NotificationCheck *m_notificationCheck;
    UserInfo *m_userInfo;
    bool m_showHidden;
    QMap<QString, QPixmap *> m_pixmapCache;
    QMap<QNetworkReply *, GUI::AppInfoLabel *> m_tmpMap;
    QMap<QString, QPixmap *> m_iconPixmapCache;

    // UI componenets
    QScrollArea *m_scrollArea;
    QWidget *m_nContainer;

};

} // namespace GUI

#endif // NOTIFICATIONCENTER_H
