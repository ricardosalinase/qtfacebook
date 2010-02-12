#ifndef FQL_GETFEEDPOSTS_H
#define FQL_GETFEEDPOSTS_H

#include "apimethod.h"

namespace API {
namespace FQL {

class GetFeedPosts : public API::Method
{
public:
    explicit GetFeedPosts(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();



protected:
    bool prepare();



};

} // namespace FQL
} // namespace API

#endif // FQL_GETFEEDPOSTS_H
