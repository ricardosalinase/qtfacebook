#include "stream_RemoveComment.h"

#include <QDebug>

namespace API {
namespace Stream {

RemoveComment::RemoveComment(QObject *parent) :
    API::Method(parent)
{
    requires("comment_id");

}

bool RemoveComment::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    qDebug() << "start: " << qName;

    m_currentText.clear();
    return true;
}

bool RemoveComment::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    qDebug() << m_currentText << "\n" << "End: " << qName;
    return true;
}

QString RemoveComment::getMethodName() {
    return "stream.removeComment";
}


} // namespace Stream
} // namespace API
