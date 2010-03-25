#ifndef STREAM_REMOVELIKE_H
#define STREAM_REMOVELIKE_H

#include "api/apimethod.h"

namespace API {
namespace Stream {

class RemoveLike : public API::Method
{
Q_OBJECT
public:
    explicit RemoveLike(QObject *parent = 0);
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

#endif // STREAM_REMOVELIKE_H
