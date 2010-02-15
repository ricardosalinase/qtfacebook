#include "streampost.h"

namespace DATA {

StreamPost::StreamPost()
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

StreamCommentList * StreamPost::getCommentList() {
    return m_commentList;
}

void StreamPost::setPoster(FbUserInfo *poster) {
    m_poster = poster;
}

FbUserInfo * StreamPost::getPoster() {
    return m_poster;
}

void StreamPost::setActorId(QString id) {
    m_actorId = id;
}

QString StreamPost::getActorId() {
    return m_actorId;
}


} // namespace DATA
