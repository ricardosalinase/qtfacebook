#ifndef STREAMCOMMENTLIST_H
#define STREAMCOMMENTLIST_H

#include <QString>
#include <QList>

namespace DATA {

class StreamComment
{
public:
    StreamComment();

    void setFromId(QString id);
    QString getFromId();
    void setTime(QString time);
    QString getTime();
    void setText(QString text);
    QString getText();
    void setCommentId(QString id);
    QString getCommentId();

private:
    QString m_fromId;
    QString m_time;
    QString m_text;
    QString m_commentId;


};


class StreamCommentList : public QList<StreamComment *>
{
public:
    StreamCommentList();
    bool canRemove();
    void canRemove(bool);
    bool canPost();
    void canPost(bool);


private:
    bool m_canRemove;
    bool m_canPost;

};

} // namespace DATA

#endif // STREAMCOMMENTLIST_H
