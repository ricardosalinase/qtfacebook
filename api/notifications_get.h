#ifndef NOTIFICATIONS_GET_H
#define NOTIFICATIONS_GET_H

#include "apimethod.h"

namespace API {
namespace Notifications {

class Get : public API::Method
{
public:
    Get(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();


};

} // namespace Notifications
} // namespace API

#endif // NOTIFICATIONS_GET_H
