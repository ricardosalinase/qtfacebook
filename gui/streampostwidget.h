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
#include <QToolButton>
#include <QTimer>
#include <QProgressBar>

#include "data/FbStreamPost.h"
#include "api/factory.h"




namespace GUI {

class StreamPostWidget : public QWidget
{
Q_OBJECT
public:
    enum RequestType { PosterPixmap, AppIcon };
    explicit StreamPostWidget(DATA::FbStreamPost *post, QWidget *parent = 0);
    ~StreamPostWidget();

signals:
    void closed(GUI::StreamPostWidget*, QString);
    void contentClicked(QString url);
public slots:

protected:
    void closeEvent ( QCloseEvent * event );

private slots:
    void gotNetworkReply(QNetworkReply *reply);
    void gotContentUpdate();
    void likeChanged(bool likes);

private:
    void getPosterPixmap();
    void getPhoto(DATA::FbStreamMedia *media);
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_photoLayout;
    QHBoxLayout *m_ageLineLayout;
    QHBoxLayout *m_linkLayout;
    QNetworkAccessManager *m_nam;
    DATA::FbStreamPost *m_post;
    QLabel *m_userPicLabel;
    QLabel *m_postLabel;
    bool m_triedBothIcons;
    QMap<QNetworkReply*, RequestType> m_outstandingNetworkRequests;
    QToolButton *m_showAddCommentButton;
    QToolButton *m_likeButton;
};

} // namespace GUI

#endif // STREAMPOSTWIDGET_H
