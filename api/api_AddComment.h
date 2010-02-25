#ifndef API_ADDCOMMENT_H
#define API_ADDCOMMENT_H

#include "apimethod.h"

namespace API {
namespace Stream {

class AddComment : public API::Method
{

public:
    explicit AddComment(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();

signals:

public slots:

};

} // namespace Stream
} // namespace API

#endif // API_ADDCOMMENT_H
