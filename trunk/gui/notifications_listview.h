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
#include <QScrollArea>
#include <QThreadStorage>
#include <QCache>


#include "userinfo.h"
#include "api/factory.h"
#include "notifications_widget.h"

namespace GUI {
namespace Notifications {

class ListView : public QWidget
{
Q_OBJECT
public:

    enum mode { ALL, RECENT, NEW };

    ListView(UserInfo *userInfo, QWidget *parent = 0);

    void restoreWindow();
    void reload(mode m);

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
    void getPixmaps();
    void cachePixmap(const QString &key, QPixmap *object);
    bool getPixmapFromCache(const QString &key, QPixmap **pm);
    UserInfo *m_userInfo;
    API::Factory *m_factory;
    QMap<QString, DATA::Notification> nMap;
    QMap<QString, QPixmap *> m_pixmapCache;
    QMap<QString, DATA::AppInfo*> *m_appInfoMap;
    QList<DATA::Notification*> *m_notificationList;
    QMap<QNetworkReply *, QString> m_tmpMap;
    bool m_showHidden;
    // UI componenets
    QScrollArea *m_scrollArea;
    QWidget *m_nContainer;




};

} // namespace Notifications
} // namespace GUI

#endif // NOTIFICATIONS_LISTVIEW_H
