#ifndef STREAMPOST_H
#define STREAMPOST_H

#include <QString>

#include "streamcommentlist.h"
#include "data/fbuserinfo.h"

namespace DATA {

class StreamPost
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
    StreamCommentList * getCommentList();
    void setPoster(FbUserInfo *poster);
    FbUserInfo * getPoster();

private:
    QString m_postId;
    QString m_sourceId;
    QString m_actorId;
    QString m_message;
    QString m_targetId;
    QString m_createdTime;
    QString m_updatedTime;
    // StreamAttachment m_attachment;
    StreamCommentList *m_commentList;
    FbUserInfo *m_poster;


};

} // namespace DATA

#endif // STREAMPOST_H
