#ifndef STREAMPOSTWIDGET_H
#define STREAMPOSTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScrollArea>
#include <QLabel>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QProgressBar>

#include "data/streampost.h"
#include "api/factory.h"
#include "userinfo.h"



namespace GUI {

class StreamPostWidget : public QWidget
{
Q_OBJECT
public:
    enum RequestType { PosterPixmap, Photo, AppIcon, LinkThumb };
    explicit StreamPostWidget(DATA::StreamPost *post, UserInfo *info, QWidget *parent = 0);
    ~StreamPostWidget();
    void scrollToBottom();
signals:
    void closed(GUI::StreamPostWidget*);
public slots:

protected:
    void closeEvent ( QCloseEvent * event );

private slots:
    void gotPosterPixmap(QNetworkReply *reply);
    void gotPhoto(QNetworkReply *reply);
    void gotContentUpdate();
    void commentButtonClicked();
    void apiStreamAddComment(API::Stream::AddComment *method);
    void apiStreamAddCommentFailed(API::Stream::AddComment *method);
    void gotComments(API::FQL::GetComments *method);
    void getCommentsFailed(API::FQL::GetComments *method);
    void getComments();

private:
    void getPosterPixmap();
    void getPhoto(DATA::FbStreamMedia *media);
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_photoLayout;
    QHBoxLayout *m_ageLineLayout;
    QHBoxLayout *m_linkLayout;
    QVBoxLayout *m_commentLayout;
    QNetworkAccessManager *m_nam;
    QNetworkAccessManager *m_nam2;
    DATA::StreamPost *m_post;
    QScrollArea *m_commentScrollArea;
    QWidget *m_commentContainer;
    QLabel *m_userPicLabel;
    QLabel *m_postLabel;
    bool m_triedBothIcons;
    QMap<QNetworkReply*, RequestType> m_outstandingNetworkRequests;
    QTextEdit *m_commentEdit;
    QPushButton *m_addCommentButton;
    API::Factory *m_factory;
    QTimer *m_commentTimer;
    QProgressBar *m_commentProgressBar;
    QWidget *m_progressWidget;
};

} // namespace GUI

#endif // STREAMPOSTWIDGET_H
