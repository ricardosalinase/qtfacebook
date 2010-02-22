#include "fql_GetStreamPosts.h"

#include <QDebug>

namespace API {
namespace FQL {

GetStreamPosts::GetStreamPosts(QObject *parent) :
        Method(parent),
        m_parseState(QUERY)
{

    m_streamPosts = new QList<DATA::StreamPost *>();
    requires("queries");
}

bool GetStreamPosts::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                  const QString &qName, const QXmlAttributes &/*attributes*/)
{

    if (m_parseState != QUERY)
    {
        if (qName == "stream_post")
        {
            m_currentStreamPost = new DATA::StreamPost();
        }
        else if (m_parseState == POSTS && qName == "comments")
        {
            m_parseState = COMMENTLIST;
        }
        else if (m_parseState == POSTS && qName == "attachment")
        {
            m_currentAttachment = new DATA::FbStreamAttachment();
            m_parseState = ATTACHMENT;
        }
        else if (m_parseState == ATTACHMENT && qName == "stream_media")
        {
            m_currentStreamMedia = new DATA::FbStreamMedia();
            m_parseState = STREAMMEDIA;
        }
        else if (m_parseState == STREAMMEDIA && qName == "photo")
        {
            m_currentPhoto = new DATA::FbPhoto();
            m_parseState = PHOTO;
        }
        else if (m_parseState == COMMENTS && qName == "comment")
        {
            m_currentStreamComment = new DATA::StreamComment();
        }
        else if (m_parseState == COMMENTOR && qName == "user")
        {
            m_currentCommentor = new DATA::FbUserInfo();
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

bool GetStreamPosts::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                const QString &qName)
{

    switch(m_parseState) {
    case QUERY:
        if (qName == "name")
        {
            if (m_currentText == "posts")
                m_parseState = POSTS;
            else if (m_currentText == "post_comments")
                m_parseState = COMMENTS;
            else if (m_currentText == "poster_info")
                m_parseState = POSTER;
            else if (m_currentText == "commentors")
                m_parseState = COMMENTOR;
            else if (m_currentText == "page_info")
                m_parseState = PAGE;
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
        else if (qName == "source_id")
            m_currentStreamPost->setSourceId(m_currentText);
        else if (qName == "target_id")
            m_currentStreamPost->setTargetId(m_currentText);
        else if (qName == "actor_id")
            m_currentStreamPost->setActorId(m_currentText);
        else if (qName == "message")
            m_currentStreamPost->setMessage(m_currentText);
        else if (qName == "created_time")
            m_currentStreamPost->setCreatedTime(m_currentText);
        else if (qName == "updated_time")
            m_currentStreamPost->setUpdatedTime(m_currentText);
        else if (qName == "attribution")
            m_currentStreamPost->setAttribution(m_currentText);
        break;
    case COMMENTLIST:
        if (qName == "can_remove")
            m_currentStreamPost->getCommentList()->canRemove(m_currentText.compare("1") == 0 ? true : false);
        else if (qName == "can_post")
            m_currentStreamPost->getCommentList()->canPost(m_currentText.compare("1") == 0 ? true : false);
        else if (qName == "comments")
            m_parseState = POSTS;
        break;
    case COMMENTS:
        if (qName == "comment")
        {
            m_postMap[m_currentStreamComment->getPostId()]->getCommentList()->append(m_currentStreamComment);
            m_commentMap.insertMulti(m_currentStreamComment->getFromId(), m_currentStreamComment);
            m_currentStreamComment = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "post_id")
            m_currentStreamComment->setPostId(m_currentText);
        else if (qName == "fromid")
            m_currentStreamComment->setFromId(m_currentText);
        else if (qName == "time")
            m_currentStreamComment->setTime(m_currentText);
        else if (qName == "text")
            m_currentStreamComment->setText(m_currentText);
        else if (qName == "id")
            m_currentStreamComment->setCommentId(m_currentText);
        break;
    case COMMENTOR:
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
        else if (qName == "pic")
            m_currentPoster->setPic(m_currentText);
        else if (qName == "pic_big")
            m_currentPoster->setPicBig(m_currentText);
        else if (qName == "uid")
            m_currentPoster->setUID(m_currentText);
        break;
    case PAGE:
        if (qName == "page")
        {
            QList<DATA::StreamPost *> pList = m_postMap.values(m_currentPage->getPageId());
            for (int i = 0; i < pList.size(); i++)
            {
                pList.at(i)->setPage(m_currentPage);
            }
            // A page can also leave comments ...
            QList<DATA::StreamComment *> cList = m_commentMap.values(m_currentPage->getPageId());
            if (cList.size() > 0)
            {
                DATA::FbUserInfo *info = new DATA::FbUserInfo();
                info->setName(m_currentPage->getName());
                info->setPicSquare(m_currentPage->getPicSquare().toString());
                info->setUID(m_currentPage->getPageId());
                for (int i = 0; i < cList.size(); i++)
                {
                    cList.at(i)->setUserInfo(info);
                }
                delete info;
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
        else if (qName == "pic")
            m_currentPage->setPic(m_currentText);
        else if (qName == "pic_big")
            m_currentPage->setPicBig(m_currentText);
        else if (qName == "pic_square")
            m_currentPage->setPicSquare(m_currentText);
        break;
    case ATTACHMENT:
        if (qName == "attachment")
        {
            m_currentStreamPost->setAttachment(m_currentAttachment);
            m_currentAttachment = 0;
            m_parseState = POSTS;
        }
        else if (qName == "name")
            m_currentAttachment->setName(m_currentText);
        else if (qName == "href")
            m_currentAttachment->setHref(m_currentText);
        else if (qName == "description")
            m_currentAttachment->setDescription(m_currentText);
        else if (qName == "caption")
            m_currentAttachment->setCaption(m_currentText);
        else if (qName == "icon")
            m_currentAttachment->setIcon(m_currentText);
        else if (qName == "fb_object_type")
            m_currentAttachment->setFbObjectType(m_currentText);
        break;
    case STREAMMEDIA:
        if (qName == "stream_media")
        {
            m_currentAttachment->addMedia(m_currentStreamMedia);
            m_currentStreamMedia = 0;
            m_parseState = ATTACHMENT;
        }
        else if (qName == "href")
            m_currentStreamMedia->setHref(m_currentText);
        else if (qName == "src")
            m_currentStreamMedia->setSrc(m_currentText);
        else if (qName == "alt")
            m_currentStreamMedia->setAlt(m_currentText);
        else if (qName == "type")
            m_currentStreamMedia->setType(m_currentText);
        break;
    case PHOTO:
        if (qName == "photo")
        {
            QVariant *v = new QVariant();
            v->setValue(*m_currentPhoto);
            m_currentStreamMedia->setMedia(v);
            delete m_currentPhoto;
            m_currentPhoto = 0;
            m_parseState = STREAMMEDIA;
        }
        else if (qName == "aid")
            m_currentPhoto->setAlbumId(m_currentText);
        else if (qName == "pid")
            m_currentPhoto->setPhotoId(m_currentText);
        else if (qName == "owner")
            m_currentPhoto->setOwnerId(m_currentText);

    }

    qDebug() << "End: " << qName << ": " << m_currentText;

    return true;
}

bool GetStreamPosts::prepare() {

   QString fql("{\"posts\":\"SELECT post_id, source_id, app_id, "
               "target_id, actor_id, message, created_time, updated_time, "
               "attachment, likes, action_links, attribution "
               "FROM stream WHERE post_id='");

   fql.append(m_argMap.take("post_id").toString());

   fql.append("'\",\"post_comments\":\"SELECT post_id, fromid, text, time, id "
              "FROM comment WHERE post_id IN (select post_id FROM #posts)\","
              "\"poster_info\":\"SELECT uid, name, pic, pic_big FROM user WHERE uid "
              "IN (SELECT actor_id FROM #posts)\","
              "\"commentors\":\"SELECT uid, name, pic_square "
              "FROM user WHERE uid IN (SELECT fromid FROM #post_comments)\","
              "\"page_info\":\"SELECT page_id, name, pic, pic_square, pic_big FROM page WHERE page_id "
              "IN (SELECT actor_id FROM #posts) OR page_id IN (SELECT fromid FROM #post_comments)\"}");

   //qDebug() << "queries: " << fql;
    m_argMap.insert("queries", fql);



    return true;
}

QString GetStreamPosts::getMethodName() {
    return "fql.multiquery.getStreamPosts";
}
// The HTML is escaped, but it's fairly limited

void GetStreamPosts::fixHtml(QString *html) {

    html->replace("&gt;",">");
    html->replace("&lt;", "<");
    html->replace("&quot;","\"");
    html->replace("&amp;","&");
    html->replace("#039;","'");

}

QList<DATA::StreamPost *> * GetStreamPosts::getStreamPosts() {
    return m_streamPosts;
}

} // namespace FQL
} // namespace API
