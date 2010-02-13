#ifndef FQL_GETFEEDPOSTS_H
#define FQL_GETFEEDPOSTS_H

#include "apimethod.h"
#include "data/streampost.h"

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
    enum ParseState { POST, COMMENTLIST, COMMENT };
    ParseState m_parseState;
    bool prepare();
    QList<DATA::StreamPost *> *m_streamPosts;
    DATA::StreamPost *m_currentStreamPost;
    DATA::StreamComment *m_currentStreamComment;
    void fixHtml(QString *html);



};

} // namespace FQL
} // namespace API

#endif // FQL_GETFEEDPOSTS_H
