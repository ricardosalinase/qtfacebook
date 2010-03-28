#ifndef FBCOMMENTMANAGER_H
#define FBCOMMENTMANAGER_H

#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QToolButton>
#include <QTextEdit>
#include <QList>
#include <QTimer>
#include <QVBoxLayout>

#include "api/factory.h"
#include "gui/FbCommentWidget.h"

#ifdef WITH_ASPELL
#include "SpellTextEdit/SpellTextEdit.h"
#else
#include <QTextEdit>
#endif



namespace GUI {

class FbCommentManager : public QWidget
{
Q_OBJECT
public:
    enum FbType { PostId, ObjectId };
    FbCommentManager(const QString& id, FbType type, bool isOwner, QWidget *parent = 0);
    ~FbCommentManager();

signals:
    void updatedComments();
public slots:
    void showAddComment();
    
private slots:
    void commentButtonClicked();
    void apiStreamAddComment(API::Stream::AddComment *method);
    void apiStreamAddCommentFailed(API::Stream::AddComment *method);
    void gotComments(API::FQL::GetComments *method);
    void getCommentsFailed(API::FQL::GetComments *method);
    void getComments();
    void userDeletedComment(GUI::FbCommentWidget *commentWidget);
    void apiStreamRemoveComment(API::Stream::RemoveComment *method);
    void apiStreamRemoveCommentFailed(API::Stream::RemoveComment *method);
    void apiCommentsAdd(API::Comments::Add *method);
    void apiCommentsAddFailed(API::Comments::Add *method);
    void apiCommentsRemove(API::Comments::Remove *method);
    void apiCommentsRemoveFailed(API::Comments::Remove *method);

private:
    FbType m_idType;
    QString m_id;
    API::Factory *m_factory;
    bool m_isOwner;
    QList<DATA::StreamComment *> m_commentList;
    QTimer *m_commentTimer;

    QScrollArea *m_commentScrollArea;
    QWidget *m_commentContainer;
    QVBoxLayout *m_commentLayout;
    QWidget *m_progressWidget;

#ifdef WITH_ASPELL
    SpellTextEdit *m_commentEdit;
#else
    QTextEdit *m_commentEdit;
#endif
    QPushButton *m_addCommentButton;
    QToolButton *m_likeButton;
    QToolButton *m_showAddCommentButton;

};

} // namespace GUI

#endif // FBCOMMENTMANAGER_H
