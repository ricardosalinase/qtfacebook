#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "data/streamcommentlist.h"

namespace GUI {

class CommentWidget : public QWidget
{
Q_OBJECT
public:
    explicit CommentWidget(DATA::StreamComment *, QWidget *parent = 0);

signals:

public slots:

private slots:
    void gotUserPicSquare(QNetworkReply *reply);

private:
    DATA::StreamComment *m_comment;
    QNetworkAccessManager *m_nam;
    void getCommentPic();

};

} // namespace GUI

#endif // COMMENTWIDGET_H
