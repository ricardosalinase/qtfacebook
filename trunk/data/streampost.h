#ifndef STREAMPOST_H
#define STREAMPOST_H

#include <QString>

#include "streamcommentlist.h"
#include "data/fbuserinfo.h"
#include "gui/notificationcenteritem.h"

namespace DATA {

    class StreamPost : public GUI::NotificationCenterItem
{
public:
    explicit StreamPost();

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
    void isHidden(bool hidden);
    bool isHidden();
    StreamCommentList * getCommentList();
    void setPoster(FbUserInfo *poster);
    FbUserInfo& getPoster();

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
    bool m_isHidden;
    // StreamAttachment m_attachment;
    StreamCommentList *m_commentList;
    FbUserInfo m_poster;


};

} // namespace DATA

#endif // STREAMPOST_H
