#ifndef FQL_GETPHOTOS_H
#define FQL_GETPHOTOS_H

#include "apimethod.h"
#include "data/FbPhoto.h"
#include "data/FbAlbum.h"


namespace API {
namespace FQL {

class GetPhotos : public API::Method
{
public:
    explicit GetPhotos(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();
    QList<DATA::FbPhoto *> * getPhotos();

private:
    enum ParseState { QUERY, PHOTO, ALBUM, USERINFO, PAGEINFO };
    ParseState m_parseState;
    bool prepare();
    DATA::FbPhoto *m_currentPhoto;
    DATA::FbAlbum *m_currentAlbum;
    DATA::FbUserInfo *m_currentUser;
    DATA::FbPageInfo *m_currentPage;
    QList<DATA::FbPhoto *> *m_photos;
    QMap<QString, DATA::FbPhoto *> m_ownerToPhotoMap;
    QMap<QString, DATA::FbPhoto *> m_albumToPhotoMap;
};

} // namespace FQL
} // namespace API

#endif // FQL_GETPHOTO_H
