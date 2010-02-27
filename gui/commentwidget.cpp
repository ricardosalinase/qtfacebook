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

CommentWidget::CommentWidget(DATA::StreamComment *comment, bool canDelete, QWidget *parent) :
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
    commentTimeHtml.prepend("<style type=\"text/css\">a { text-decoration: none; }</style>"
                            "<font style=\"font-size : 8px;\">");

    if (canDelete)
    {
        commentTimeHtml.append(" - <a href=\"deleteMe\">delete</a>");
    }

    commentTimeHtml.append("</font>");


    QLabel *commentLabel = new QLabel();
    commentLabel->setText(commentHtml);
    commentLabel->setWordWrap(true);

    QLabel *commentTimeLabel = new QLabel(commentTimeHtml);
    connect(commentTimeLabel,SIGNAL(linkActivated(QString)),
            this, SLOT(linkClicked(QString)));
    layout->addWidget(commentLabel,0,1,Qt::AlignTop);
    layout->addWidget(commentTimeLabel,1,1,Qt::AlignBottom);
    layout->setColumnStretch(1,1);

    // By setting this placeholder, it makes the parent widget size correctly
    // before we have the actual pic
    m_picLabel = new QLabel();
    m_picLabel->setMinimumHeight(50);
    layout->addWidget(m_picLabel,0,0,2,1,Qt::AlignTop);
    setLayout(layout);

    getCommentPic();
}

CommentWidget::~CommentWidget() {
    //delete m_comment;
}

void CommentWidget::linkClicked(QString link) {

    if (link == "deleteMe")
    {
        emit userClickedDelete(this);
    }
}

DATA::StreamComment * CommentWidget::getComment() {
    return m_comment;
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
        m_picLabel->setPixmap(*p);
        //((QGridLayout *)layout())->addWidget(l,0,0,2,1,Qt::AlignTop);
    }
}

void CommentWidget::gotUserPicSquare(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        m_picLabel->setPixmap(p);
        UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
        cache->cachePixmap(m_comment->getFromId(),UTIL::FbUserPicCache::PicSquare,
                           reply->request().url(),p);
        //((QGridLayout *)layout())->addWidget(l,0,0,2,1,Qt::AlignTop);

    }

}



} // namespace GUI
