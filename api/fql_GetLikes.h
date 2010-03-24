#ifndef FQL_GETLIKES_H
#define FQL_GETLIKES_H

#include "api/apimethod.h"
#include "data/fbuserinfo.h"

namespace API {
namespace FQL {

class GetLikes : public API::Method
{

public:
    explicit GetLikes(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
            const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
            const QString &qName);
    QString getMethodName();
    QList<DATA::FbUserInfo *> getLikers();

signals:

public slots:

private:
    bool prepare();
    DATA::FbUserInfo *m_currentUser;
    QList<DATA::FbUserInfo *> m_likerList;

};

} // namespace FQL
} // namespace API

#endif // FQL_GETLIKES_H
