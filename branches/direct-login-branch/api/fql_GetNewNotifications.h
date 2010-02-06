#ifndef FQL_GETNEWNOTIFICATIONS_H
#define FQL_GETNEWNOTIFICATIONS_H

#include "apimethod.h"
#include "data/notification.h"
#include <QStringList>

using namespace DATA;

namespace API {
namespace FQL {

class GetNewNotifications : public API::Method
{

public:
    explicit GetNewNotifications(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();

    QList<Notification*> * getNotificationList();
    QStringList * getAppIds();

private:
    QList<Notification *> *m_notifications;
    QStringList *m_appIds;
    Notification *m_currentNotification;
    void fixHtml(QString *html);
    bool prepare();
};

} // namespace FQL
} // namespace API

#endif // FQL_GETNEWNOTIFICATIONS_H
