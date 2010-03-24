#include "fql_GetLikes.h"

#include <QDebug>

namespace API {
namespace FQL {

GetLikes::GetLikes(QObject *parent) :
    API::Method(parent)
{
}

bool GetLikes::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                            const QString &qName, const QXmlAttributes &/*attributes*/)
{

    if (qName == "user")
        m_currentUser = new DATA::FbUserInfo();

    qDebug() << "start: " << qName;
    m_currentText.clear();
    return true;
}

bool GetLikes::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                            const QString &qName)
{
    if (qName == "user")
    {
        m_likerList.append(m_currentUser);
        m_currentUser = 0;
    }
    else if (qName == "uid")
        m_currentUser->setUID(m_currentText);
    else if (qName == "name")
        m_currentUser->setName(m_currentText);


    qDebug() << "End: " << qName << " : " << m_currentText;
    return true;
}

QList<DATA::FbUserInfo *> GetLikes::getLikers()
{
    return m_likerList;
}

QString GetLikes::getMethodName()
{
    return "fql.query.getLikes";
}

bool GetLikes::prepare()
{
    QString fql("SELECT uid, name FROM user WHERE uid "
                "IN (SELECT user_id FROM like WHERE object_id=");

    if (m_argMap.contains("object_id"))
        fql.append(m_argMap.take("object_id").toString() + ")");
    else
    {
        m_errStr = "Missing object_id";
        return false;
    }

    m_argMap.insert("query", fql);

    qDebug() << fql;

    return true;

}

} // namespace FQL
} // namespace API
