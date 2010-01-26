#ifndef USERS_GETLOGGEDINUSER_H
#define USERS_GETLOGGEDINUSER_H

#include "apimethod.h"

namespace API {
namespace Users {

class GetLoggedInUser : public API::Method
{
public:
    GetLoggedInUser(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();


};

} // namespace Users
} // namespace API

#endif // USERS_GETLOGGEDINUSER_H
