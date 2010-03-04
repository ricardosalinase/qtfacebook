#include "comments_Remove.h"

namespace API {
namespace Comments {

Remove::Remove(QObject *parent) :
    API::Method(parent)
{
    requires("comment_id");
    requires("object_id");

}

bool Remove::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    m_currentText.clear();
    return true;
}

bool Remove::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    return true;
}

QString Remove::getMethodName() {
    return "comments.remove";
}
} // namespace Comments
} // namespace API
