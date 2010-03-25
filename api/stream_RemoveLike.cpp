#include "stream_RemoveLike.h"

#include <QDebug>

namespace API {
namespace Stream {

RemoveLike::RemoveLike(QObject *parent) :
    API::Method(parent)
{
    requires("post_id");
}

bool RemoveLike::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    qDebug() << "Start: " << qName;
    m_currentText.clear();
    return true;
}

bool RemoveLike::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    qDebug() << "End: " << qName << " : " << m_currentText;
    return true;
}


QString RemoveLike::getMethodName() {
    return "stream.removeLike";
}


} // namespace Steam
} // namespace API


