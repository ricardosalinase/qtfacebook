#ifndef FQL_GETFEEDPOSTS_H
#define FQL_GETFEEDPOSTS_H

#include "apimethod.h"
#include "data/streampost.h"
#include "data/fbuserinfo.h"
#include "data/FbPageInfo.h"
#include "data/FbStreamAttachment.h"
#include "data/FbPhoto.h"

namespace API {
namespace FQL {

class GetStreamPosts : public API::Method
{
public:
    explicit GetStreamPosts(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();
    QList<DATA::StreamPost *> * getStreamPosts();



private:
    enum ParseState { QUERY, POSTS, COMMENTLIST, 
                      COMMENTS, POSTER, COMMENTOR, PAGE,
                    ATTACHMENT, STREAMMEDIA, PHOTO};
    ParseState m_parseState;
    bool prepare();
    QMap<QString, DATA::StreamPost* > m_postMap;
    QMap<QString, DATA::StreamComment* > m_commentMap;
    QList<DATA::StreamPost *> *m_streamPosts;
    DATA::StreamPost *m_currentStreamPost;
    DATA::StreamComment *m_currentStreamComment;
    DATA::FbUserInfo *m_currentCommentor;
    void fixHtml(QString *html);
    QString m_currentQueryName;
    DATA::FbUserInfo *m_currentPoster;
    DATA::FbPageInfo *m_currentPage;
    DATA::FbStreamAttachment *m_currentAttachment;
    DATA::FbPhoto *m_currentPhoto;
    DATA::FbStreamMedia *m_currentStreamMedia;

};

} // namespace FQL
} // namespace API

#endif // FQL_GETFEEDPOSTS_H
