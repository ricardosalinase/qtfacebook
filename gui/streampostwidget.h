#ifndef STREAMPOSTWIDGET_H
#define STREAMPOSTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScrollArea>
#include <QLabel>
#include <QCloseEvent>


#include "data/streampost.h"


namespace GUI {

class StreamPostWidget : public QWidget
{
Q_OBJECT
public:
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

private:
    void getPosterPixmap();
    QNetworkAccessManager *m_nam;
    DATA::StreamPost *m_post;
    QScrollArea *m_commentScrollArea;
    QWidget *m_commentContainer;
    QLabel *m_userPicLabel;
    QLabel *m_postLabel;

};

} // namespace GUI

#endif // STREAMPOSTWIDGET_H
