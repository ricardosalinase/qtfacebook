#include "fql_getstreampostinfo.h"

#include <QDebug>

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

    QString fql("{\"posts\":\"SELECT post_id, actor_id, created_time, is_hidden "
               "FROM stream WHERE filter_key = 'nf' ");

    if (m_argMap.contains("get_hidden"))
    {
        m_argMap.take("get_hidden");
    }
    else
    {
        fql.append("AND is_hidden = 0 ");
    }

    if (m_argMap.contains("start_time"))
        fql.append("AND created_time > " + m_argMap.take("start_time").toString());

    fql.append("\",\"poster_info\":\"SELECT uid, name, pic_square FROM user WHERE uid "
               "IN (SELECT actor_id FROM #posts)\",\"page_info\":\"SELECT page_id, name, "
               "pic_square FROM page WHERE page_id IN (SELECT actor_id FROM #posts)\"}");

    qDebug() << fql;
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
        else if (m_parseState == PAGE && qName == "page")
        {
            m_currentPage = new DATA::FbPageInfo();
        }
    }

    qDebug() << "Start: " << qName;

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
            else if (m_currentText == "page_info")
                m_parseState = PAGE;
        }
        break;
    case POSTS:
        if (qName == "stream_post")
        {
            m_streamPosts->append(m_currentStreamPost);
            m_postMap.insert(m_currentStreamPost->getPostId(), m_currentStreamPost);
            m_postMap.insertMulti(m_currentStreamPost->getActorId(), m_currentStreamPost);
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
        else if (qName == "is_hidden")
            m_currentStreamPost->isHidden(m_currentText.compare("1") == 0 ? true : false );
        break;
    case POSTER:
        if (qName == "user")
        {
            QList<DATA::StreamPost *> pList = m_postMap.values(m_currentPoster->getUID());
            for (int i = 0; i < pList.size(); i++)
            {
                pList.at(i)->setPoster(m_currentPoster);
            }
            delete m_currentPoster;
            m_currentPoster = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "name")
            m_currentPoster->setName(m_currentText);
        else if (qName == "uid")
            m_currentPoster->setUID(m_currentText);
        else if (qName == "pic_square")
            m_currentPoster->setPicSquare(m_currentText);
        break;
    case PAGE:
        if (qName == "page")
        {
            QList<DATA::StreamPost *> pList = m_postMap.values(m_currentPage->getPageId());
            for (int i = 0; i < pList.size(); i++)
            {
                pList.at(i)->setPage(m_currentPage);
            }
            delete m_currentPage;
            m_currentPage = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "name")
            m_currentPage->setName(m_currentText);
        else if (qName == "page_id")
            m_currentPage->setPageId(m_currentText);
        else if (qName == "pic_square")
            m_currentPage->setPicSquare(m_currentText);
        break;
    }

    qDebug() << m_currentText;
    qDebug() << "End: " << qName;

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
