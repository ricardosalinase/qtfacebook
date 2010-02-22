#ifndef STREAMPOSTWIDGET_H
#define STREAMPOSTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScrollArea>
#include <QLabel>
#include <QCloseEvent>
#include <QHBoxLayout>


#include "data/streampost.h"


namespace GUI {

class StreamPostWidget : public QWidget
{
Q_OBJECT
public:
    enum RequestType { PosterPixmap, Photo, AppIcon };
    explicit StreamPostWidget(DATA::StreamPost *post, QWidget *parent = 0);
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

private:
    void getPosterPixmap();
    void getPhoto(DATA::FbStreamMedia *media);
    QHBoxLayout *m_photoLayout;
    QHBoxLayout *m_ageLineLayout;
    QNetworkAccessManager *m_nam;
    QNetworkAccessManager *m_nam2;
    DATA::StreamPost *m_post;
    QScrollArea *m_commentScrollArea;
    QWidget *m_commentContainer;
    QLabel *m_userPicLabel;
    QLabel *m_postLabel;
    QMap<QNetworkReply*, RequestType> m_outstandingNetworkRequests;

};

} // namespace GUI

#endif // STREAMPOSTWIDGET_H
