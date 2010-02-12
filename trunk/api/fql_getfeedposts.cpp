#include "fql_getfeedposts.h"

namespace API {
namespace FQL {

GetFeedPosts::GetFeedPosts(QObject *parent) :
        Method(parent)
{
}

bool GetFeedPosts::startElement(const QString &namespaceURI, const QString &localName,
                  const QString &qName, const QXmlAttributes &attributes)
{


    return true;
}

bool GetFeedPosts::endElement(const QString &namespaceURI, const QString &localName,
                const QString &qName)
{

    return true;
}

bool GetFeedPosts::prepare() {

    return true;
}

QString GetFeedPosts::getMethodName() {
    return "fbl.query.getFeedPosts";
}

} // namespace FQL
} // namespace API
