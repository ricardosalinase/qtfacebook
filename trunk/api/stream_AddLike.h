#ifndef STREAM_ADDLIKE_H
#define STREAM_ADDLIKE_H

#include "api/apimethod.h"

namespace API {
namespace Stream {

class AddLike : public API::Method
{

public:
    explicit AddLike(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();

signals:

public slots:

};

} // namespace Steam
} // namespace API

#endif // STREAM_ADDLIKE_H
