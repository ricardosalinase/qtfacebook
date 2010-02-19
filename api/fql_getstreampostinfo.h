#ifndef FQL_GETSTREAMPOSTINFO_H
#define FQL_GETSTREAMPOSTINFO_H

#include <QString>
#include <QList>

#include "api/apimethod.h"
#include "data/streampost.h"
#include "data/fbuserinfo.h"

namespace API {
namespace FQL {

class GetStreamPostInfo : public API::Method
{
public:
    explicit GetStreamPostInfo(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();
    QList<DATA::StreamPost *> * getStreamPosts();

private:
    enum ParseState { QUERY, POSTS, POSTER };
    ParseState m_parseState;
    bool prepare();
    QMap<QString, DATA::StreamPost* > m_postMap;
    DATA::StreamPost *m_currentStreamPost;
    DATA::FbUserInfo *m_currentPoster;
    QList<DATA::StreamPost *> *m_streamPosts;

};

} // namespace FQL
} // namespace API

#endif // FQL_GETSTREAMPOSTINFO_H
