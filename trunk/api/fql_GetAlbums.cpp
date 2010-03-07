#include "fql_GetAlbums.h"

#include <QDebug>


namespace API {
namespace FQL {

GetAlbums::GetAlbums(QObject *parent) :
    API::Method(parent),
    m_parseState(QUERY)
{
    m_albums = new QList<DATA::FbAlbum *>();
    requires("queries");

}

bool GetAlbums::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    if (m_parseState != QUERY)
    {

        if (qName == "album")
            m_currentAlbum = new DATA::FbAlbum();
        else if (qName == "user")
            m_currentUser = new DATA::FbUserInfo();
        else if (qName == "page")
            m_currentPage = new DATA::FbPageInfo();
    }

    qDebug() << "Start: " << m_currentText;


    m_currentText.clear();
    return true;
}

bool GetAlbums::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    switch(m_parseState)
    {
        case QUERY:
            if (qName == "name")
            {
                if (m_currentText == "albums")
                    m_parseState = ALBUM;
                else if (m_currentText == "user_info")
                    m_parseState = USERINFO;
                else if (m_currentText == "page_info")
                    m_parseState = PAGEINFO;
            }
            break;

        case ALBUM:
            if (qName == "album")
            {
                m_ownerToAlbumMap.insertMulti(m_currentAlbum->getOwnerID(),
                                              m_currentAlbum);
                m_albums->append(m_currentAlbum);
                m_currentAlbum = 0;
            }
            else if (qName == "fql_result")
                m_parseState = QUERY;
            else if (qName == "aid")
                m_currentAlbum->setAlbumId(m_currentText);
            else if (qName == "name")
                m_currentAlbum->setAlbumName(m_currentText);
            else if (qName == "owner")
                m_currentAlbum->setOwnerId(m_currentText);
            else if (qName == "cover_pid")
                m_currentAlbum->setCoverPID(m_currentText);
            else if (qName == "created")
                m_currentAlbum->setCreatedTime(m_currentText);
            else if (qName == "modified")
                m_currentAlbum->setModifiedTime(m_currentText);
            else if (qName == "description")
                m_currentAlbum->setDescription(m_currentText);
            else if (qName == "location")
                m_currentAlbum->setLocation(m_currentText);
            else if (qName == "size")
                m_currentAlbum->setNumPhotos(m_currentText);
            else if (qName == "modified_major")
                m_currentAlbum->setModifiedMajorTime(m_currentText);
            else if (qName == "type")
                m_currentAlbum->setAlbumType(m_currentText);
            else if (qName == "object_id")
                m_currentAlbum->setObjectId(m_currentText);
            break;

        case USERINFO:
            if (qName == "user")
            {
                QList<DATA::FbAlbum *> pList = m_ownerToAlbumMap.values(m_currentUser->getUID());
                for (int i = 0; i < pList.size(); i++)
                {
                    pList.at(i)->setUserInfo(m_currentUser);
                }
                delete m_currentUser;
                m_currentUser = 0;
            }
            else if (qName == "fql_result")
                m_parseState = QUERY;
            else if (qName == "name")
                m_currentUser->setName(m_currentText);
            else if (qName == "pic")
                m_currentUser->setPic(m_currentText);
            else if (qName == "pic_big")
                m_currentUser->setPicBig(m_currentText);
            else if (qName == "pic_square")
                m_currentUser->setPicSquare(m_currentText);
            else if (qName == "uid")
                m_currentUser->setUID(m_currentText);
            break;

        case PAGEINFO:
            if (qName == "page")
            {
                QList<DATA::FbAlbum *> pList = m_ownerToAlbumMap.values(m_currentPage->getPageId());
                for (int i = 0; i < pList.size(); i++)
                {
                    pList.at(i)->setPageInfo(m_currentPage);
                }
                delete m_currentPage;
                m_currentUser = 0;
            }
            else if (qName == "fql_result")
                m_parseState = QUERY;
            else if (qName == "name")
                m_currentPage->setName(m_currentText);
            else if (qName == "page_id")
                m_currentPage->setPageId(m_currentText);
            else if (qName == "pic")
                m_currentPage->setPic(m_currentText);
            else if (qName == "pic_big")
                m_currentPage->setPicBig(m_currentText);
            else if (qName == "pic_square")
                m_currentPage->setPicSquare(m_currentText);
            break;

    }


    return true;
}

QString GetAlbums::getMethodName() {
    return "fql.multiquery.getAlbums";
}

QList<DATA::FbAlbum *> * GetAlbums::getAlbums() {
    return m_albums;
}

bool GetAlbums::prepare() {

    QString fql("{\"albums\":\"SELECT aid, owner, cover_pid, name, created, modified, "
                "description, location, size, modified_major, type, object_id "
                "FROM album WHERE ");

    if (m_argMap.contains("aid"))
    {
        fql.append("aid = '" + m_argMap.take("aid").toString() + "'");
    }
    else if (m_argMap.contains("owner"))
    {
        fql.append("owner = '" + m_argMap.take("owner").toString() + "'");
    }
    else
    {
        m_errStr = "Missing aid or owner";
        return false;
    }

    fql.append("\",\"user_info\":\"SELECT uid, name, pic_square, pic, pic_big FROM user "
               "WHERE uid IN (SELECT owner FROM #albums)\","
               "\"page_info\":\"SELECT page_id, name, pic, pic_square, pic_big "
               "FROM page WHERE page_id IN (SELECT owner FROM #albums)\"}");

    m_argMap.insert("queries", fql);

    qDebug() << fql;

    return true;
}


} // namespace FQL
} // namespace API
