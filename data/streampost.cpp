#include "streampost.h"

namespace DATA {

StreamPost::StreamPost() :
    m_isFromUser(true)
{
    m_commentList = new StreamCommentList();
}

void StreamPost::setPostId(QString id) {
    m_postId = id;
}

QString StreamPost::getPostId() {
    return m_postId;
}

void StreamPost::setSourceId(QString id) {
    m_sourceId = id;
}

QString StreamPost::getSourceId() {
    return m_sourceId;
}

void StreamPost::setMessage(QString m) {
    m_message = m;
}

QString StreamPost::getMessage() {
    return m_message;
}

void StreamPost::setTargetId(QString tid) {
    m_targetId = tid;
}

QString StreamPost::getTargetId() {
    return m_targetId;
}

void StreamPost::setCreatedTime(QString t) {
    m_createdTime = t;
}

QString StreamPost::getCreatedTime() {
    return m_createdTime;
}

void StreamPost::setUpdatedTime(QString t) {
    m_updatedTime = t;
}

QString StreamPost::getUpdatedTime() {
    return m_updatedTime;
}

void StreamPost::isHidden(bool hidden) {
    m_isHidden = hidden;
}

bool StreamPost::isHidden() {
    return m_isHidden;
}

StreamCommentList * StreamPost::getCommentList() {
    return m_commentList;
}

void StreamPost::setPoster(FbUserInfo *poster) {
    m_poster = *poster;
}

FbUserInfo & StreamPost::getPoster() {
    return m_poster;
}

void StreamPost::setPage(FbPageInfo *page) {
    m_isFromUser = false;
    m_page = *page;
}

FbPageInfo & StreamPost::getPage() {
    return m_page;
}

void StreamPost::setActorId(QString id) {
    m_actorId = id;
}

QString StreamPost::getActorId() {
    return m_actorId;
}

bool StreamPost::isFromUser() {
    return m_isFromUser;
}

void StreamPost::isFromUser(bool fromUser) {
    m_isFromUser = fromUser;
}

bool StreamPost::hasAttachment() {
    return m_hasAttachment;
}

const DATA::FbStreamAttachment * StreamPost::getAttachment() {
    return m_attachment;
}

void StreamPost::setAttachment(const DATA::FbStreamAttachment *attachment) {
    m_attachment = attachment;
}

// NotificationCenterItem Interface
QString StreamPost::getNavigationHtml() {

    QString html("There is a <a href=\"streamPost:");
    html.append(m_postId + "\">");
    html.append("new post</a> from ");

    if (m_isFromUser)
        html.append(m_poster.getName());
    else
        html.append(m_page.getName());

    html.append(" on your page.");

    return html;
}

QString StreamPost::getNotificationCenterId() {
    return m_postId;
}


} // namespace DATA
