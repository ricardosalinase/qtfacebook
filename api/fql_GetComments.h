#ifndef FQL_GETCOMMENTS_H
#define FQL_GETCOMMENTS_H

#include "apimethod.h"
#include "data/streamcommentlist.h"
#include "data/fbuserinfo.h"

namespace API {
namespace FQL {

class GetComments : public API::Method
{
public:
    enum ParseState { QUERY, COMMENTS, COMMENTORS, PAGE_COMMENTORS };
    explicit GetComments(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
            const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
            const QString &qName);
    QString getMethodName();
    QList<DATA::StreamComment *> getCommentList();

private:
    bool prepare();
    DATA::StreamComment *m_currentComment;
    DATA::FbUserInfo *m_currentCommentor;
    QMap<QString, DATA::StreamComment *> m_commentMap;
    QList<DATA::StreamComment *> m_commentList;
    ParseState m_parseState;


};

} // namespace FQL
} // namespace API

#endif // FQL_GETCOMMENTS_H
