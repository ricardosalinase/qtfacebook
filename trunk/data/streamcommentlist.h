#ifndef STREAMCOMMENTLIST_H
#define STREAMCOMMENTLIST_H

#include <QString>
#include <QList>
#include <QMap>
#include <QUrl>

#include <data/fbuserinfo.h>

namespace DATA {

class StreamComment
{
public:
    StreamComment();
    ~StreamComment();

    void setPostId(QString id);
    QString getPostId();
    void setFromId(QString id);
    QString getFromId();
    void setTime(QString time);
    QString getTime();
    void setText(QString text);
    QString getText();
    void setCommentId(QString id);
    QString getCommentId();
    QString getUserName();
    QUrl getUserPicSquare();

    void setUserInfo(FbUserInfo *userInfo);


private:
    QString m_postId;
    QString m_fromId;
    QString m_time;
    QString m_text;
    QString m_commentId;
    FbUserInfo m_fbUserInfo;


};


class StreamCommentList : public QList<StreamComment *>
{
public:
    StreamCommentList();
    bool canRemove();
    void canRemove(bool);
    bool canPost();
    void canPost(bool);
    void addUserInfo(FbUserInfo *info);


private:
    bool m_canRemove;
    bool m_canPost;


};

} // namespace DATA

#endif // STREAMCOMMENTLIST_H
