#include "fql_GetStreamPosts.h"

namespace API {
namespace FQL {

GetStreamPosts::GetStreamPosts(QObject *parent) :
        Method(parent),
        m_parseState(POST)
{

    requires("query");
}

bool GetStreamPosts::startElement(const QString &namespaceURI, const QString &localName,
                  const QString &qName, const QXmlAttributes &attributes)
{
    if (qName == "stream_post")
        m_currentStreamPost = new DATA::StreamPost();
    else if (qName == "comments")
        m_parseState = COMMENTLIST;
    else if (qName == "comment") {
        m_currentStreamComment = new DATA::StreamComment();
        m_parseState = COMMENT;
    }


    m_currentText.clear();
    return true;
}

bool GetStreamPosts::endElement(const QString &namespaceURI, const QString &localName,
                const QString &qName)
{

    switch(m_parseState) {
    case POST:
        if (qName == "stream_post") {
            m_streamPosts->append(m_currentStreamPost);
            m_currentStreamPost = 0;
        } else if (qName == "post_id")
            m_currentStreamPost->setPostId(m_currentText);
        else if (qName == "source_id")
            m_currentStreamPost->setSourceId(m_currentText);
        else if (qName == "target_id")
            m_currentStreamPost->setTargetId(m_currentText);
        else if (qName == "message")
            m_currentStreamPost->setMessage(m_currentText);
        else if (qName == "created_time")
            m_currentStreamPost->setCreatedTime(m_currentText);
        else if (qName == "updated_time")
            m_currentStreamPost->setUpdatedTime(m_currentText);
        break;
    case COMMENTLIST:
        if (qName == "can_remove")
            m_currentStreamPost->getCommentList()->canRemove(m_currentText.compare("1") == 0 ? true : false);
        else if (qName == "can_post")
            m_currentStreamPost->getCommentList()->canPost(m_currentText.compare("1") == 0 ? true : false);
        else if (qName == "comments")
            m_parseState = POST;
        break;
    case COMMENT:
        if (qName == "comment") {
            m_currentStreamPost->getCommentList()->append(m_currentStreamComment);
            m_currentStreamComment = 0;
            m_parseState = COMMENTLIST;
        } else if (qName == "fromid")
            m_currentStreamComment->setFromId(m_currentText);
        else if (qName == "time")
            m_currentStreamComment->setTime(m_currentText);
        else if (qName == "text")
            m_currentStreamComment->setText(m_currentText);
        else if (qName == "id")
            m_currentStreamComment->setCommentId(m_currentText);
        break;
    }

    return true;
}

bool GetStreamPosts::prepare() {

    QString fql("SELECT post_id, source_id, target_id, message, created_time, updated_time, attachment, comments, likes, action_links, attribution FROM stream WHERE post_id=\"");

    fql.append(m_argMap.take("post_id").toString() + "\"");

    m_argMap.insert("query", fql);



    return true;
}

QString GetStreamPosts::getMethodName() {
    return "fbl.query.getStreamPosts";
}
// The HTML is escaped, but it's fairly limited

void GetStreamPosts::fixHtml(QString *html) {

    html->replace("&gt;",">");
    html->replace("&lt;", "<");
    html->replace("&quot;","\"");
    html->replace("&amp;","&");
    html->replace("#039;","'");

}

} // namespace FQL
} // namespace API
