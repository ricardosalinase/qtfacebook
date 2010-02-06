#ifndef NOTIFICATIONS_MARKREAD_H
#define NOTIFICATIONS_MARKREAD_H

#include "apimethod.h"

namespace API {
namespace Notifications {

class MarkRead : public Method
{
public:
    MarkRead(QObject *parent = 0);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();
    bool successful();
    QString getNotificationIds();

private:
    bool m_succeeded;

};

} // namespace Notifications
} // namespace API

#endif // NOTIFICATIONS_MARKREAD_H
