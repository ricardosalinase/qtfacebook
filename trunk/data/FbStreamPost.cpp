#include "FbStreamPost.h"

namespace DATA {

FbStreamPost::FbStreamPost() :
    m_userLikes(false),
    m_isFromUser(true),
    m_hasAttachment(false),
    m_attachment(0)
{
    m_commentList = new StreamCommentList();
}

void FbStreamPost::setPostId(QString id) {
    m_postId = id;
}

QString FbStreamPost::getPostId() {
    return m_postId;
}

void FbStreamPost::setSourceId(QString id) {
    m_sourceId = id;
}

QString FbStreamPost::getSourceId() {
    return m_sourceId;
}

void FbStreamPost::setMessage(QString m) {
    m_message = m;
}

QString FbStreamPost::getMessage() {
    return m_message;
}

void FbStreamPost::setTargetId(QString tid) {
    m_targetId = tid;
}

QString FbStreamPost::getTargetId() {
    return m_targetId;
}

void FbStreamPost::setCreatedTime(QString t) {
    m_createdTime = t;
}

QString FbStreamPost::getCreatedTime() {
    return m_createdTime;
}

void FbStreamPost::setUpdatedTime(QString t) {
    m_updatedTime = t;
}

QString FbStreamPost::getUpdatedTime() {
    return m_updatedTime;
}

void FbStreamPost::isHidden(bool hidden) {
    m_isHidden = hidden;
}

bool FbStreamPost::isHidden() {
    return m_isHidden;
}

StreamCommentList * FbStreamPost::getCommentList() {
    return m_commentList;
}

void FbStreamPost::setPoster(FbUserInfo *poster) {
    m_poster = *poster;
}

FbUserInfo & FbStreamPost::getPoster() {
    return m_poster;
}

void FbStreamPost::setTarget(FbUserInfo *target) {
    m_target = *target;
}

FbUserInfo& FbStreamPost::getTarget() {
    return m_target;
}

void FbStreamPost::setPage(FbPageInfo *page) {
    m_isFromUser = false;
    m_page = *page;
}

FbPageInfo & FbStreamPost::getPage() {
    return m_page;
}

void FbStreamPost::setActorId(QString id) {
    m_actorId = id;
}

QString FbStreamPost::getActorId() {
    return m_actorId;
}

void FbStreamPost::setAttribution(QString attribution) {
    m_attribution = attribution;
}

QString FbStreamPost::getAttribution() {
    return m_attribution;
}

void FbStreamPost::setAppId(QString appId) {
    m_appId = appId;
}

QString FbStreamPost::getAppId() {
    return m_appId;
}

void FbStreamPost::userLikes(bool likes)
{
    m_userLikes = likes;
}

bool FbStreamPost::userLikes()
{
    return m_userLikes;
}

void FbStreamPost::setAppInfo(AppInfo *info) {
    m_appInfo = *info;
}

AppInfo& FbStreamPost::getAppInfo() {
    return m_appInfo;
}

bool FbStreamPost::isFromUser() {
    return m_isFromUser;
}

void FbStreamPost::isFromUser(bool fromUser) {
    m_isFromUser = fromUser;
}

bool FbStreamPost::hasAttachment() {
    return m_hasAttachment;
}

DATA::FbStreamAttachment * FbStreamPost::getAttachment() {
    return m_attachment;
}

void FbStreamPost::setAttachment(DATA::FbStreamAttachment *attachment) {
    m_attachment = attachment;
    m_hasAttachment = true;
}


// NotificationCenterItem Interface
QString FbStreamPost::getNavigationHtml() {

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

QString FbStreamPost::getNotificationCenterId() {
    return m_postId;
}


} // namespace DATA
