#ifndef COMMENTS_ADD_H
#define COMMENTS_ADD_H

#include "apimethod.h"

namespace API {
namespace Comments {

class Add : public API::Method
{

public:
    explicit Add(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();


};

} // namespace Comments
} // namespace API

#endif // COMMENTS_ADD_H
