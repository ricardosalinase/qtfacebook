#ifndef NOTIFICATIONS_GETLIST_H
#define NOTIFICATIONS_GETLIST_H

#include "apimethod.h"
#include "data/notification.h"
#include "data/appinfo.h"
#include <QMap>

using namespace DATA;

namespace API {
namespace Notifications {

class GetList : public API::Method
{
public:
    GetList(QObject *parent = 0);
    ~GetList() {};
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();

    QList<Notification*> * getNotificationList();
    QMap<QString, AppInfo*> * getAppInfoMap();

private:
    QList<Notification *> *m_notifications;
    QMap<QString, AppInfo* > *m_appInfo;
    Notification *m_currentNotification;
    AppInfo *m_currentAppInfo;


};

} // namespace Notifications
} // namespace API

#endif // NOTIFICATIONS_GETLIST_H
