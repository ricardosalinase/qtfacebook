#include "FbCommentWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QNetworkRequest>
#include <QPixmap>
#include <QDebug>
#include <QDesktopServices>

#include "util/agestring.h"
#include "util/fbuserpiccache.h"
#include "util/HyperLink.h"

namespace GUI {

FbCommentWidget::FbCommentWidget(DATA::StreamComment *comment, bool canDelete, QWidget *parent) :
    QWidget(parent),
    m_comment(comment)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotUserPicSquare(QNetworkReply*)));

    QGridLayout *layout = new QGridLayout();

    QString commentHtml = "<b>" + comment->getUserName() + ":</b> " +
                          UTIL::hyperLink(comment->getText(), 50);

    QString commentTimeHtml = UTIL::ageString(comment->getTime());
    commentTimeHtml.prepend("<style type=\"text/css\">a { text-decoration: none; }</style>"
                            "<font style=\"font-size : 8px;\">");

    if (canDelete)
    {
        commentTimeHtml.append(" - <a href=\"deleteMe\">delete</a>");
    }

    commentTimeHtml.append("</font>");


    QLabel *commentLabel = new QLabel();
    QFont newFont = font();
    newFont.setPointSize(newFont.pointSize() - 2);
    commentLabel->setText(commentHtml);
    commentLabel->setFont(newFont);
    commentLabel->setWordWrap(true);
    connect(commentLabel, SIGNAL(linkActivated(QString)),
            this, SLOT(linkClicked(QString)));

    QLabel *commentTimeLabel = new QLabel(commentTimeHtml);
    connect(commentTimeLabel,SIGNAL(linkActivated(QString)),
            this, SLOT(linkClicked(QString)));
    layout->addWidget(commentLabel,0,1,Qt::AlignTop);
    layout->addWidget(commentTimeLabel,1,1,Qt::AlignBottom);
    layout->setColumnStretch(1,1);

    // By setting this placeholder, it makes the parent widget size correctly
    // before we have the actual pic
    m_picLabel = new QLabel();
    m_picLabel->setMinimumSize(50,50);;
    layout->addWidget(m_picLabel,0,0,2,1,Qt::AlignTop);
    setLayout(layout);

    getCommentPic();
}

FbCommentWidget::~FbCommentWidget() {
    //delete m_comment;
}

void FbCommentWidget::linkClicked(QString link) {

    if (link == "deleteMe")
    {
        emit userClickedDelete(this);
    }
    else
    {
        QUrl encodedUrl;
        encodedUrl.setEncodedUrl(link.toUtf8());
        QDesktopServices::openUrl(encodedUrl);
    }
}

DATA::StreamComment * FbCommentWidget::getComment() {
    return m_comment;
}

void FbCommentWidget::getCommentPic() {


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
        delete p;
    }
}

void FbCommentWidget::gotUserPicSquare(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        m_picLabel->setPixmap(p);
        UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
        cache->cachePixmap(m_comment->getFromId(),UTIL::FbUserPicCache::PicSquare,
                           reply->request().url(),p);


    }

}



} // namespace GUI
