#include "fql_GetComments.h"

namespace API {
namespace FQL {

GetComments::GetComments(QObject *parent) :
            Method(parent),
            m_parseState(QUERY)
{

}

bool GetComments::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
        const QString &qName, const QXmlAttributes &/*attributes*/)
{
    if (m_parseState != QUERY)
    {
        if (qName == "comment")
            m_currentComment = new DATA::StreamComment();
        else if (qName == "user" || qName == "page")
            m_currentCommentor = new DATA::FbUserInfo();

    }


    m_currentText.clear();
    return true;
}

bool GetComments::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
        const QString &qName)
{
    switch(m_parseState) {
    case QUERY:
        if (qName == "name")
        {
            if (m_currentText == "comments")
                m_parseState = COMMENTS;
            else if (m_currentText == "commentors")
                m_parseState = COMMENTORS;
            else if (m_currentText == "page_commentors")
                m_parseState = PAGE_COMMENTORS;
        }
        break;
    case COMMENTS:
        if (qName == "comment")
        {
            m_commentList.append(m_currentComment);
            m_commentMap.insertMulti(m_currentComment->getFromId(), m_currentComment);
            m_currentComment = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "post_id")
            m_currentComment->setPostId(m_currentText);
        else if (qName == "object_id")
            m_currentComment->setFbObjectId(m_currentText);
        else if (qName == "fromid")
            m_currentComment->setFromId(m_currentText);
        else if (qName == "time")
            m_currentComment->setTime(m_currentText);
        else if (qName == "text")
            m_currentComment->setText(m_currentText);
        else if (qName == "id")
            m_currentComment->setCommentId(m_currentText);
        break;
    case COMMENTORS:
        if (qName == "user")
        {
            QList<DATA::StreamComment *> cList = m_commentMap.values(m_currentCommentor->getUID());
            for (int i = 0; i < cList.size(); i++)
            {
                cList.at(i)->setUserInfo(m_currentCommentor);
            }
            delete m_currentCommentor;
            m_currentCommentor = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "name")
            m_currentCommentor->setName(m_currentText);
        else if (qName == "uid")
            m_currentCommentor->setUID(m_currentText);
        else if (qName == "pic_square")
            m_currentCommentor->setPicSquare(m_currentText);
        break;
    case PAGE_COMMENTORS:
        if (qName == "page")
        {
            QList<DATA::StreamComment *> cList = m_commentMap.values(m_currentCommentor->getUID());
            if (cList.size() > 0)
            {
                for (int i = 0; i < cList.size(); i++)
                {
                    cList.at(i)->setUserInfo(m_currentCommentor);
                }
                delete m_currentCommentor;
                m_currentCommentor = 0;
            }
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "name")
            m_currentCommentor->setName(m_currentText);
        else if (qName == "page_id")
            m_currentCommentor->setUID(m_currentText);
        else if (qName == "pic_square")
            m_currentCommentor->setPicSquare(m_currentText);
        break;
    }


    return true;
}

QString GetComments::getMethodName() {
    return "fql.multiquery.getComments";
}

QList<DATA::StreamComment *> GetComments::getCommentList()
{
    return m_commentList;
}

bool GetComments::prepare() {

    QString fql("{\"comments\":\"SELECT post_id, object_id, fromid, time, text, id FROM comment WHERE ");

    if (m_argMap.contains("post_id"))
        fql.append("post_id='" + m_argMap.take("post_id").toString());
    else if (m_argMap.contains("object_id"))
        fql.append("object_id='" + m_argMap.take("object_id").toString());
    else
    {
        m_errStr = "post_id or object_id not supplied";
        return false;
    }

    fql.append("'\",\"commentors\":\"SELECT uid, name, pic_square FROM user WHERE uid IN "
               "(SELECT fromid FROM #comments)\",\"page_commentors\":"
               "\"SELECT page_id, name, pic_square FROM page WHERE page_id IN (SELECT fromid FROM #comments)\"}");

    m_argMap.insert("queries", fql);



    return true;
}

} // namespace FQL
} // namespace API
