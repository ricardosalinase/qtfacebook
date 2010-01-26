#ifndef NOTIFICATIONS_GETLIST_H
#define NOTIFICATIONS_GETLIST_H

#include "apimethod.h"

namespace API {
namespace Notifications {

class GetList : public API::Method
{
public:
    GetList(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();


};

} // namespace Notifications
} // namespace API

#endif // NOTIFICATIONS_GETLIST_H
