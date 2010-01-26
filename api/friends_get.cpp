#include "friends_get.h"

namespace API {
namespace Friends {

Get::Get(QObject *parent) : Method(parent)
{


}

// m_currentText is defined in the base API::Method class, as is the method that
// fills it during the XML parsing
bool Get::startElement(const QString &namespaceURI, const QString &localName,
                       const QString &qName, const QXmlAttributes &attributes) {



    m_currentText.clear();
    return true;

}

QString Get::getMethodName() {
    return "friends.get";
}

// m_currentText is defined in the base API::Method class, as is the method that
// fills it during the XML parsing

bool Get::endElement(const QString &namespaceURI, const QString &localName,
                     const QString &qName) {

    if (qName == "uid")
        m_uids.append(m_currentText);

    return true;

}

QStringList Get::getFriendIdList() {
    return m_uids;
}


} // namespace Freinds
} // namespace API
