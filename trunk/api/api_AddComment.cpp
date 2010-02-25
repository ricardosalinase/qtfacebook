#include "api_AddComment.h"

namespace API {
namespace Stream {

AddComment::AddComment(QObject *parent) :
    API::Method(parent)
{
    requires("post_id");
    requires("comment");

}

bool AddComment::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{



}

bool AddComment::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

}

QString AddComment::getMethodName() {
    return "stream.addComment";
}


} // namespace Stream
} // namespace API
