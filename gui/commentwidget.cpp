#include "commentwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QNetworkRequest>
#include <QPixmap>
#include <QDebug>

#include "util/agestring.h"
#include "util/fbuserpiccache.h"

namespace GUI {

CommentWidget::CommentWidget(DATA::StreamComment *comment, QWidget *parent) :
    QWidget(parent),
    m_comment(comment)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotUserPicSquare(QNetworkReply*)));

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

    setLayout(layout);

    getCommentPic();
}

CommentWidget::~CommentWidget() {
    //delete m_comment;
}

void CommentWidget::getCommentPic() {


    UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();

    QPixmap *p = cache->getPixmap(m_comment->getFromId(), UTIL::FbUserPicCache::PicSquare,
                                 m_comment->getUserPicSquare() );

    if (p == 0)
    {
        QNetworkRequest nr;
        nr.setUrl(m_comment->getUserPicSquare());
        m_nam->get(nr);
    }
    else
    {
        QLabel *l = new QLabel();
        l->setPixmap(*p);
        ((QGridLayout *)layout())->addWidget(l,0,0,2,1,Qt::AlignTop);
    }
}

void CommentWidget::gotUserPicSquare(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);
        UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
        cache->cachePixmap(m_comment->getFromId(),UTIL::FbUserPicCache::PicSquare,
                           reply->request().url(),p);
        ((QGridLayout *)layout())->addWidget(l,0,0,2,1,Qt::AlignTop);

    }

}



} // namespace GUI
