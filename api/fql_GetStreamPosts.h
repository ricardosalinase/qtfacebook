#ifndef FQL_GETFEEDPOSTS_H
#define FQL_GETFEEDPOSTS_H

#include "apimethod.h"
#include "data/FbStreamPost.h"
#include "data/fbuserinfo.h"
#include "data/FbPageInfo.h"
#include "data/FbStreamAttachment.h"
#include "data/FbPhoto.h"
#include "data/FbStreamAttachmentProperty.h"
#include "data/appinfo.h"

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
    QList<DATA::FbStreamPost *> * getStreamPosts();



private:
    enum ParseState {   QUERY, POSTS, COMMENTLIST,
                        POSTER, PAGE, ATTACHMENT,
                        STREAMMEDIA, PHOTO, PROPERTY,
                        APPINFO, MEDIADETAIL };
    ParseState m_parseState;
    bool prepare();
    QMap<QString, DATA::FbStreamPost* > m_postMap;
    QMap<QString, DATA::FbStreamPost* > m_appToPostMap;
    QMap<QString, DATA::StreamComment* > m_commentMap;
    QMap<QString, DATA::FbStreamPost* > m_targetToPostMap;
    QList<DATA::FbStreamPost *> *m_streamPosts;
    DATA::FbStreamPost *m_currentStreamPost;
    DATA::StreamComment *m_currentStreamComment;
    DATA::FbUserInfo *m_currentCommentor;
    void fixHtml(QString *html);
    QString m_currentQueryName;
    DATA::FbUserInfo *m_currentPoster;
    DATA::FbPageInfo *m_currentPage;
    DATA::FbStreamAttachment *m_currentAttachment;
    DATA::FbPhoto *m_currentPhoto;
    DATA::FbStreamMedia *m_currentStreamMedia;
    DATA::FbStreamAttachmentProperty *m_currentProperty;
    DATA::AppInfo *m_currentAppInfo;

};

} // namespace FQL
} // namespace API

#endif // FQL_GETFEEDPOSTS_H
