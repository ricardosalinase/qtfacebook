#ifndef STREAM_REMOVECOMMENT_H
#define STREAM_REMOVECOMMENT_H

#include "apimethod.h"

namespace API {
namespace Stream {

class RemoveComment : public API::Method
{
public:
    explicit RemoveComment(QObject *parent = 0);
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

#endif // STREAM_REMOVECOMMENT_H
