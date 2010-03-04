#ifndef COMMENTS_REMOVE_H
#define COMMENTS_REMOVE_H

#include "api/apimethod.h"

namespace API {
namespace Comments {

class Remove : public API::Method
{
public:
    explicit Remove(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();


};

} // namespace Comments
} // namespace API

#endif // COMMENTS_REMOVE_H
