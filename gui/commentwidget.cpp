#include "commentwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QNetworkRequest>
#include <QPixmap>

#include "util/agestring.h"

namespace GUI {

CommentWidget::CommentWidget(DATA::StreamComment *comment, QWidget *parent) :
    QWidget(parent),
    m_comment(comment)
{

    QGridLayout *layout = new QGridLayout();

    QString commentHtml = comment->getUserName() + ": " +
                          comment->getText();

    QString commentTimeHtml = UTIL::ageString(comment->getTime());
    commentTimeHtml.prepend("<font style=\"font-size : 8px;\">");
    commentTimeHtml.append("</font>");


    QLabel *commentLabel = new QLabel(commentHtml);
    commentLabel->setWordWrap(true);
    QLabel *commentTimeLabel = new QLabel(commentTimeHtml);
    layout->addWidget(commentLabel,0,1,Qt::AlignTop);
    layout->addWidget(commentTimeLabel,1,1,Qt::AlignBottom);
    layout->setColumnStretch(1,1);
    getCommentPic();

    setLayout(layout);



}

void CommentWidget::getCommentPic() {

    m_nam = new QNetworkAccessManager(this);

    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotUserPicSquare(QNetworkReply*)));

    QNetworkRequest nr;
    nr.setUrl(m_comment->getUserPicSquare());
    m_nam->get(nr);

}

void CommentWidget::gotUserPicSquare(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);
        ((QGridLayout *)layout())->addWidget(l,0,0,2,1,Qt::AlignTop);

    }

}



} // namespace GUI
