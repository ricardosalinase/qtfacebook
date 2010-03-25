#include "stream_AddLike.h"

#include <QDebug>

namespace API {
namespace Stream {

AddLike::AddLike(QObject *parent) :
    API::Method(parent)
{
    requires("post_id");
}

bool AddLike::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    qDebug() << "Start: " << qName;
    m_currentText.clear();
    return true;
}

bool AddLike::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    qDebug() << "End: " << qName << " : " << m_currentText;
    return true;
}


QString AddLike::getMethodName() {
    return "stream.addLike";
}


} // namespace Steam
} // namespace API
