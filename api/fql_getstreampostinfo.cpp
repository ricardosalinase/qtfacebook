#include "fql_getstreampostinfo.h"

namespace API {
namespace FQL {

GetStreamPostInfo::GetStreamPostInfo(QObject *parent) :
    Method(parent),
    m_parseState(QUERY)
{
    m_streamPosts = new QList<DATA::StreamPost *>();
    requires("queries");
}

bool GetStreamPostInfo::prepare() {

   QString fql("{\"posts\":\"SELECT post_id, actor_id, created_time "
               "FROM stream WHERE filter_key = 'nf' AND is_hidden = 0\","
                "\"poster_info\":\"SELECT uid, name FROM user WHERE uid "
              "IN (SELECT actor_id FROM #posts)\"}");


    m_argMap.insert("queries", fql);



    return true;
}

bool GetStreamPostInfo::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    if (m_parseState != QUERY)
    {
        if (qName == "stream_post")
        {
            m_currentStreamPost = new DATA::StreamPost();
        }
        else if (m_parseState == POSTER && qName == "user")
        {
            m_currentPoster = new DATA::FbUserInfo();

        }
    }

    m_currentText.clear();
    return true;
}

bool GetStreamPostInfo::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    switch(m_parseState) {
    case QUERY:
        if (qName == "name")
        {
            if (m_currentText == "posts")
                m_parseState = POSTS;
            else if (m_currentText == "poster_info")
                m_parseState = POSTER;
        }
        break;
    case POSTS:
        if (qName == "stream_post")
        {
            m_streamPosts->append(m_currentStreamPost);
            m_postMap.insert(m_currentStreamPost->getPostId(), m_currentStreamPost);
            m_postMap.insert(m_currentStreamPost->getActorId(), m_currentStreamPost);
            m_currentStreamPost = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "post_id")
            m_currentStreamPost->setPostId(m_currentText);
        else if (qName == "actor_id")
            m_currentStreamPost->setActorId(m_currentText);
        else if (qName == "created_time")
            m_currentStreamPost->setCreatedTime(m_currentText);
        break;
    case POSTER:
        if (qName == "user")
        {
            m_postMap[m_currentPoster->getUID()]->setPoster(m_currentPoster);
            m_currentPoster = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "name")
            m_currentPoster->setName(m_currentText);
        else if (qName == "uid")
            m_currentPoster->setUID(m_currentText);
        break;
    }



    return true;
}

QString GetStreamPostInfo::getMethodName() {
    return "fql.multiquery.getStreamPostInfo";
}

QList<DATA::StreamPost *> * GetStreamPostInfo::getStreamPosts() {
    return m_streamPosts;
}

} // namespace FQL
} // namespace API
