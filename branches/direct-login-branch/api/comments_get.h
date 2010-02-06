#ifndef COMMENTS_GET_H
#define COMMENTS_GET_H

#include "apimethod.h"

namespace API {
namespace Comments {

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

} // namespace Comments
} // namespace API

#endif // COMMENTS_GET_H
