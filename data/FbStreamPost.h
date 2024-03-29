#ifndef FBSTREAMPOST_H
#define FBSTREAMPOST_H

#include <QString>

#include "streamcommentlist.h"
#include "data/fbuserinfo.h"
#include "data/FbPageInfo.h"
#include "data/FbStreamAttachment.h"
#include "data/appinfo.h"
#include "gui/notificationcenteritem.h"

namespace DATA {

    class FbStreamPost : public GUI::NotificationCenterItem
{
public:
    explicit FbStreamPost();

    void setPostId(QString id);
    QString getPostId();
    void setSourceId(QString id);
    QString getSourceId();
    void setMessage(QString m);
    QString getMessage();
    void setTargetId(QString tid);
    QString getTargetId();
    void setCreatedTime(QString t);
    QString getCreatedTime();
    void setUpdatedTime(QString t);
    QString getUpdatedTime();
    void setActorId(QString id);
    QString getActorId();
    void setAttribution(QString attribution);
    QString getAttribution();
    void setAppId(QString appId);
    QString getAppId();
    void userLikes(bool likes);
    bool userLikes();
    void setAppInfo(AppInfo *);
    AppInfo& getAppInfo();
    void isHidden(bool hidden);
    bool isHidden();
    StreamCommentList * getCommentList();
    void setPoster(FbUserInfo *poster);
    FbUserInfo& getPoster();
    void setTarget(FbUserInfo *target);
    FbUserInfo& getTarget();
    void setPage(FbPageInfo *page);
    FbPageInfo& getPage();
    bool isFromUser();
    void isFromUser(bool fromUser);
    bool hasAttachment();
    DATA::FbStreamAttachment * getAttachment();
    void setAttachment(DATA::FbStreamAttachment *attachment);
    bool canPostComments();
    void canPostComments(bool);
    bool canDeleteComments();
    void canDeleteComments(bool);


    // NotificationCenterItem Interface
    QString getNavigationHtml();
    inline ItemType getNotificationCenterItemType() { return NotificationCenterItem::StreamPost; }
    QString getNotificationCenterId();


private:
    QString m_postId;
    QString m_sourceId;
    QString m_actorId;
    QString m_message;
    QString m_targetId;
    QString m_createdTime;
    QString m_updatedTime;
    QString m_attribution;
    QString m_appId;
    bool m_userLikes;
    AppInfo m_appInfo;
    bool m_isHidden;
    // StreamAttachment m_attachment;
    StreamCommentList *m_commentList;
    FbUserInfo m_poster;
    FbUserInfo m_target;
    FbPageInfo m_page;
    bool m_isFromUser;
    bool m_hasAttachment;
    DATA::FbStreamAttachment *m_attachment;


};

} // namespace DATA

#endif // FBSTREAMPOST_H
