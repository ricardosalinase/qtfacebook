#ifndef FBCOMMENTWIDGET_H
#define FBCOMMENTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QPushButton>

#include "data/streamcommentlist.h"

namespace GUI {

class FbCommentWidget : public QWidget
{
Q_OBJECT
public:
    explicit FbCommentWidget(DATA::StreamComment *, bool canDelete = false, QWidget *parent = 0);
    ~FbCommentWidget();
    DATA::StreamComment * getComment();
signals:
    void userClickedDelete(GUI::FbCommentWidget *comment);
public slots:

private slots:
    void gotUserPicSquare(QNetworkReply *reply);
    void linkClicked(QString link);

private:
    DATA::StreamComment *m_comment;
    QNetworkAccessManager *m_nam;
    void getCommentPic();
    QLabel *m_picLabel;
    QPushButton *m_deleteButton;

};

} // namespace GUI

#endif // FBCOMMENTWIDGET_H
