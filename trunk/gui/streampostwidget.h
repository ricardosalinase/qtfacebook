#ifndef STREAMPOSTWIDGET_H
#define STREAMPOSTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScrollArea>


#include "data/streampost.h"


namespace GUI {

class StreamPostWidget : public QWidget
{
Q_OBJECT
public:
    explicit StreamPostWidget(DATA::StreamPost *post, QWidget *parent = 0);
    void scrollToBottom();
signals:

public slots:

private slots:
    void gotPosterPixmap(QNetworkReply *reply);

private:
    void getPosterPixmap();
    QNetworkAccessManager *m_nam;
    DATA::StreamPost *m_post;
    QScrollArea *m_commentScrollArea;
    QWidget *m_commentContainer;
};

} // namespace GUI

#endif // STREAMPOSTWIDGET_H
