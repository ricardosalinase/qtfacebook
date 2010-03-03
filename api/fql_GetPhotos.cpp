#include "fql_GetPhotos.h"

#include <QDebug>

namespace API {
namespace FQL {

GetPhotos::GetPhotos(QObject *parent) :
        Method(parent),
        m_parseState(QUERY)
{
    m_photos = new QList<DATA::FbPhoto *>();
    requires("queries");
}

bool GetPhotos::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    if (m_parseState != QUERY)
    {

        if (qName == "photo")
            m_currentPhoto = new DATA::FbPhoto();
        else if (qName == "album")
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

bool GetPhotos::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{
    switch(m_parseState)
    {
        case QUERY:
            if (qName == "name")
            {
                if (m_currentText == "photos")
                    m_parseState = PHOTO;
                else if (m_currentText == "album_info")
                    m_parseState = ALBUM;
                else if (m_currentText == "user_info")
                    m_parseState = USERINFO;
                else if (m_currentText == "page_info")
                    m_parseState = PAGEINFO;
            }
            break;

        case PHOTO:
            if (qName == "photo")
            {
                m_ownerToPhotoMap.insertMulti(m_currentPhoto->getOwnerId(), m_currentPhoto);
                m_albumToPhotoMap.insertMulti(m_currentPhoto->getAlbumId(), m_currentPhoto);
                m_photos->append(m_currentPhoto);
                m_currentPhoto = 0;
            }
            else if (qName == "fql_result")
                m_parseState = QUERY;
            else if (qName == "pid")
                m_currentPhoto->setPhotoId(m_currentText);
            else if (qName == "aid")
                m_currentPhoto->setAlbumId(m_currentText);
            else if (qName == "owner")
                m_currentPhoto->setOwnerId(m_currentText);
            else if (qName == "src")
                m_currentPhoto->setSrc(m_currentText);
            else if (qName == "src_small")
                m_currentPhoto->setSrcSmall(m_currentText);
            else if (qName == "src_big")
                m_currentPhoto->setSrcBig(m_currentText);
            else if (qName == "caption")
                m_currentPhoto->setCaption(m_currentText);
            else if (qName == "created")
                m_currentPhoto->setCreatedTime(m_currentText);
            else if (qName == "object_id")
                m_currentPhoto->setObjectId(m_currentText);
            break;
        case ALBUM:
            if (qName == "album")
            {
                QList<DATA::FbPhoto *> pList = m_albumToPhotoMap.values(m_currentAlbum->getAlbumId());
                for (int i = 0; i < pList.size(); i++)
                {
                    pList.at(i)->setAlbumName(m_currentAlbum->getAlbumName());
                }
                delete m_currentAlbum;
                m_currentAlbum = 0;
            }
            else if (qName == "fql_result")
                m_parseState = QUERY;
            else if (qName == "aid")
                m_currentAlbum->setAlbumId(m_currentText);
            else if (qName == "name")
                m_currentAlbum->setAlbumName(m_currentText);
            break;
        case USERINFO:
            if (qName == "user")
            {
                QList<DATA::FbPhoto *> pList = m_ownerToPhotoMap.values(m_currentUser->getUID());
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
                QList<DATA::FbPhoto *> pList = m_ownerToPhotoMap.values(m_currentPage->getPageId());
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

    qDebug() << "End: " << qName << " : " << m_currentText;
    return true;
}

QString GetPhotos::getMethodName() {
    return "fql.multiquery.getPhotos";
}

QList<DATA::FbPhoto *> * GetPhotos::getPhotos() {
    return m_photos;
}

bool GetPhotos::prepare() {

    QString fql("{\"photos\":\"SELECT pid, aid, owner, src_small, src, src_big, caption, "
                "created, object_id FROM photo WHERE ");

    if (m_argMap.contains("pid"))
    {
        fql.append("pid = '" + m_argMap.take("pid").toString() + "'");
    }
    else if (m_argMap.contains("aid"))
    {
        fql.append("aid = '" + m_argMap.take("aid").toString() + "'");
    }
    else
    {
        m_errStr = "Missing pid or aid";
        return false;
    }

    fql.append("\",\"album_info\":\"SELECT aid, name FROM album WHERE "
               "aid IN (SELECT aid FROM #photos)\","
               "\"user_info\":\"SELECT uid, name, pic_square, pic, pic_big FROM user "
               "WHERE uid IN (SELECT owner FROM #photos)\","
               "\"page_info\":\"SELECT page_id, name, pic, pic_square, pic_big "
               "FROM page WHERE page_id IN (SELECT owner FROM #photos)\"}");

    m_argMap.insert("queries", fql);

    qDebug() << fql;

    return true;

}

} // namespace FQL
} // namespace API
