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
        else if (m_parseState == ATTACHMENT && qName == "stream_property")
        {
            m_currentProperty = new DATA::FbStreamAttachmentProperty();
            m_parseState = PROPERTY;
        }
        else if (m_parseState == STREAMMEDIA && qName == "photo")
        {
            m_currentPhoto = new DATA::FbPhoto();
            m_parseState = PHOTO;
        }
        else if (m_parseState == STREAMMEDIA && qName == "video")
        {
            m_parseState = MEDIADETAIL;
        }
        else if (m_parseState == POSTER && qName == "user")
        {
            m_currentPoster = new DATA::FbUserInfo();
        }
        else if (m_parseState == PAGE && qName == "page")
        {
            m_currentPage = new DATA::FbPageInfo();
        }
        else if (m_parseState == APPINFO && qName == "app_info")
            m_currentAppInfo = new DATA::AppInfo();

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
            else if (m_currentText == "poster_info")
                m_parseState = POSTER;
            else if (m_currentText == "page_info")
                m_parseState = PAGE;
            else if (m_currentText == "app_icons")
                m_parseState = APPINFO;
        }
        break;
    case POSTS:
        if (qName == "stream_post")
        {
            m_streamPosts->append(m_currentStreamPost);
            m_postMap.insert(m_currentStreamPost->getPostId(), m_currentStreamPost);
            m_postMap.insertMulti(m_currentStreamPost->getActorId(), m_currentStreamPost);
            m_appToPostMap.insertMulti(m_currentStreamPost->getAppId(), m_currentStreamPost);
            if (m_currentStreamPost->getTargetId().compare("") != 0)
                m_targetToPostMap.insertMulti(m_currentStreamPost->getTargetId(), m_currentStreamPost);
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
        else if (qName == "app_id")
            m_currentStreamPost->setAppId(m_currentText);
        break;
    case COMMENTLIST:
        if (qName == "can_remove")
            m_currentStreamPost->getCommentList()->canRemove(m_currentText.compare("1") == 0 ? true : false);
        else if (qName == "can_post")
            m_currentStreamPost->getCommentList()->canPost(m_currentText.compare("1") == 0 ? true : false);
        else if (qName == "comments")
            m_parseState = POSTS;
        break;
    case POSTER:
        if (qName == "user")
        {
            QList<DATA::StreamPost *> pList = m_postMap.values(m_currentPoster->getUID());
            for (int i = 0; i < pList.size(); i++)
            {
                pList.at(i)->setPoster(m_currentPoster);
            }
            pList.clear();
            pList = m_targetToPostMap.values(m_currentPoster->getUID());
            for (int i = 0; i < pList.size(); i++)
            {
                pList.at(i)->setTarget(m_currentPoster);
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
        else if (qName == "pic_square")
            m_currentPoster->setPicSquare(m_currentText);
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
    case APPINFO:
        if (qName == "app_info")
        {
            QList<DATA::StreamPost *> pList = m_appToPostMap.values(m_currentAppInfo->getAppId());
            for (int i = 0; i < pList.size(); i++)
            {
                pList.at(i)->setAppInfo(m_currentAppInfo);
            }
            delete m_currentAppInfo;
            m_currentAppInfo = 0;
        }
        else if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "app_id")
            m_currentAppInfo->setAppId(m_currentText);
        else if (qName == "icon_url")
            m_currentAppInfo->setIconUrl(m_currentText);
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
        else if (qName == "fb_object_id")
            m_currentAttachment->setFbObjectId(m_currentText);
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
    case PROPERTY:
        if (qName == "stream_property")
        {
            m_currentAttachment->addProperty(m_currentProperty);
            m_currentProperty = 0;
            m_parseState = ATTACHMENT;
        }
        else if (qName == "name")
            m_currentProperty->setName(m_currentText);
        else if (qName == "text")
            m_currentProperty->setText(m_currentText);
        else if (qName == "href")
            m_currentProperty->setHref(m_currentText);
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
        {
            m_currentPhoto->setAlbumId(m_currentText);
            m_currentStreamMedia->getMediaDetail().insert("aid", m_currentText);
        }
        else if (qName == "pid")
        {
            m_currentPhoto->setPhotoId(m_currentText);
            m_currentStreamMedia->getMediaDetail().insert("pid", m_currentText);
        }
        else if (qName == "owner")
            m_currentPhoto->setOwnerId(m_currentText);
        break;
    case MEDIADETAIL:
        if (qName == "video")
        {
            m_parseState = STREAMMEDIA;
        }
        else
            m_currentStreamMedia->getMediaDetail().insert(qName, m_currentText);
        break;
    }

    qDebug() << "End: " << qName << ": " << m_currentText;

    return true;
}

bool GetStreamPosts::prepare() {

   QString fql("{\"posts\":\"SELECT post_id, source_id, app_id, "
               "target_id, actor_id, message, created_time, updated_time, "
               "attachment, likes, action_links, attribution "
               "FROM stream WHERE filter_key = 'nf' ");

    if (m_argMap.contains("post_id"))
        fql.append("AND post_id = '" + m_argMap.take("post_id").toString() + "' ");

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


    fql.append("\",\"poster_info\":\"SELECT uid, name, pic_square, pic, pic_big FROM user WHERE uid "
               "IN (SELECT actor_id FROM #posts) OR uid IN (SELECT target_id FROM #posts)\","
              "\"page_info\":\"SELECT page_id, name, pic, pic_square, pic_big FROM page WHERE page_id "
              "IN (SELECT actor_id FROM #posts)\","
              "\"app_icons\":\"SELECT app_id, icon_url FROM application "
              "WHERE app_id IN (SELECT app_id FROM #posts)\"}");

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
