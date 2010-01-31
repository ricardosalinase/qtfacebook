#ifndef NOTIFICATIONS_LISTVIEW_H
#define NOTIFICATIONS_LISTVIEW_H

#include <QWidget>
#include <QThread>
#include <QEvent>
#include <QByteArray>
#include <QCloseEvent>
#include <QMap>
#include <QDialog>
#include <QWebView>

#include "userinfo.h"
#include "api/factory.h"
#include "api/notification.h"
#include "notifications_widget.h"

namespace GUI {
namespace Notifications {

class ListView : public QDialog
{
Q_OBJECT
public:
    ListView(UserInfo *userInfo, QWidget *parent = 0);

    void restoreWindow();

signals:
    void gotAllPixmaps();

public slots:
    void apiNotificationsGetList(API::Notifications::GetList *method);
    void navigate(QUrl url);
    void gotPixmap(QNetworkReply *reply);
    void displayResults();

protected:
    void closeEvent ( QCloseEvent * event );

private:
    UserInfo *m_userInfo;
    API::Factory *m_factory;
    QMap<QString, API::Notifications::Notification> nMap;
    void getPixmaps();
    void getNotifications(bool getAll = false);

    QMap<QString, API::Notifications::AppInfo*> m_appInfoMap;
    QList<API::Notifications::Notification> m_nList;
    QMap<QNetworkReply *, QString> m_tmpMap;

    // UI componenets
    QWidget *m_nContainer;

};

} // namespace Notifications
} // namespace GUI

#endif // NOTIFICATIONS_LISTVIEW_H
