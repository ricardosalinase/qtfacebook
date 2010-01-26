#ifndef FRIENDS_GET_H
#define FRIENDS_GET_H

#include "apimethod.h"

namespace API {
namespace Friends {

class Get : public API::Method
{
public:
    Get();

    // Overridden from QXmlDefaultHandler
    bool startElement(const QString &namespaceURI, const QString &localName,
                                    const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                                  const QString &qName);

    QStringList getFriendIdList();

private:
    QStringList m_uids;

};

} // namespace Freinds
} // namespace API

#endif // FRIENDS_GET_H
