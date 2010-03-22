#include "comments_Add.h"

namespace API {
namespace Comments {

Add::Add(QObject *parent) :
    API::Method(parent)
{
    requires("object_id");
    requires("text");
}

bool Add::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &/*qName*/, const QXmlAttributes &/*attributes*/)
{

    m_currentText.clear();
    return true;
}

bool Add::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &/*qName*/)
{

    return true;
}

QString Add::getMethodName() {
    return "comments.add";
}



} // namespace Comments
} // namespace API
