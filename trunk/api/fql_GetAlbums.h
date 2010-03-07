#ifndef FQL_GETALBUMS_H
#define FQL_GETALBUMS_H

#include "api/apimethod.h"
#include "data/FbAlbum.h"

namespace API {
namespace FQL {

class GetAlbums : public API::Method
{

public:
    explicit GetAlbums(QObject *parent = 0);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();
    QList<DATA::FbAlbum *> * getAlbums();

private:
    enum ParseState { QUERY, ALBUM, USERINFO, PAGEINFO };
    ParseState m_parseState;
    bool prepare();
    DATA::FbAlbum *m_currentAlbum;
    DATA::FbUserInfo *m_currentUser;
    DATA::FbPageInfo *m_currentPage;
    QList<DATA::FbAlbum *> *m_albums;
    QMap<QString, DATA::FbAlbum *> m_ownerToAlbumMap;

};

} // namespace FQL
} // namespace API

#endif // FQL_GETALBUMS_H
