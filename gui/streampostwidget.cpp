#include "streampostwidget.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDateTime>
#include <QDebug>
#include <QScrollBar>

#include "util/agestring.h"
#include "commentwidget.h"
#include "util/fbuserpiccache.h"
#include "data/FbStreamAttachment.h"


namespace GUI {

StreamPostWidget::StreamPostWidget(DATA::StreamPost *post, QWidget *parent) :
    QWidget(parent),
    m_post(post)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPosterPixmap(QNetworkReply*)));
    m_nam2 = new QNetworkAccessManager(this);
    connect(m_nam2,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPhoto(QNetworkReply*)));


    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *contentLayout = new QVBoxLayout();
    if (post->isFromUser())
        this->setWindowTitle(post->getPoster().getName());
    else
        this->setWindowTitle(post->getPage().getName());

    if (post->getMessage() != "")
    {
        QLabel *message = new QLabel(post->getMessage());
        message->setWordWrap(true);
        contentLayout->addWidget(message,0,Qt::AlignTop);
    }

    QString ageHtml = "<style type=\"text/css\">a { text-decoration: none; }</style>"
                      "<font style=\"font-size : 8pt;\">" +
                      UTIL::ageString(post->getCreatedTime()) + " ";
    if (post->getAttribution() != "" && !post->getAttribution().startsWith("via"))
        ageHtml.append("via ");
    ageHtml.append(post->getAttribution() + "</font>");

    QLabel *age = new QLabel(ageHtml);
    m_ageLineLayout = new QHBoxLayout();
    m_ageLineLayout->insertWidget(1,age,1);





    if (post->hasAttachment())
    {
        DATA::FbStreamAttachment *attachment = post->getAttachment();

        // Get the app Icon
        if (attachment->getIcon().toString() != "")
        {
            QNetworkRequest nr;
            nr.setUrl(attachment->getIcon());
            QNetworkReply *reply = m_nam2->get(nr);
            m_outstandingNetworkRequests.insert(reply, AppIcon);
        }

        QString type = attachment->getFbObjectType();
        if (type == "album" || type == "photo")
        {
            m_photoLayout = new QHBoxLayout();
            contentLayout->addLayout(m_photoLayout);
            QList<DATA::FbStreamMedia *> mList = attachment->getMedia();

            for (int i = 0; i < mList.size(); i++)
            {
                QNetworkRequest nr;
                DATA::FbStreamMedia *m = mList.at(i);
                QUrl url = m->getSrc();
                nr.setUrl(url);
                QNetworkReply *reply = m_nam2->get(nr);
                m_outstandingNetworkRequests.insert(reply, Photo);
            }

            if (type == "album")
            {
                QString albumText = "Album: " + attachment->getName() + "<BR>" +
                                    attachment->getCaption();

                contentLayout->addWidget(new QLabel(albumText));
            }

        }



    }

    contentLayout->addLayout(m_ageLineLayout);
    contentLayout->addStretch();

    m_commentContainer = new QWidget();
    m_commentScrollArea = new QScrollArea();

    QVBoxLayout *commentLayout = new QVBoxLayout();


    DATA::StreamCommentList *cList = post->getCommentList();


    for (int i = cList->size() - 1; i >= 0; i--)
    {
        CommentWidget *cw = new CommentWidget(cList->at(i));
        commentLayout->insertWidget(0,cw);
    }

    commentLayout->addStretch();

    m_commentContainer->setLayout(commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);
    m_commentScrollArea->setWidgetResizable(true);
    if (!cList->size())
        m_commentScrollArea->setVisible(false);

    
    contentLayout->addWidget(m_commentScrollArea,1);
    mainLayout->insertLayout(1,contentLayout,1);
    setLayout(mainLayout);

    this->setMinimumWidth(640);



    getPosterPixmap();

}

StreamPostWidget::~StreamPostWidget() {
    delete m_post;
}

void StreamPostWidget::gotPhoto(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        RequestType t = m_outstandingNetworkRequests.take(reply);

        if (t == Photo)
        {
            QPixmap p;
            p.loadFromData(reply->readAll());
            QLabel *l = new QLabel();
            l->setPixmap(p);
            m_photoLayout->addWidget(l);
            qDebug() << "Adding photo to layout";
        }
        else if (t == AppIcon)
        {
            QPixmap p;
            p.loadFromData(reply->readAll());
            QLabel *l = new QLabel();
            l->setPixmap(p);
            m_ageLineLayout->insertWidget(0,l,0);

        }
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }

    reply->deleteLater();

}

void StreamPostWidget::getPosterPixmap() {

    QString uid;
    QUrl url;

    if (m_post->isFromUser())
    {
        uid = m_post->getPoster().getUID();
        url = m_post->getPoster().getPicBig();
    }
    else
    {
        uid = m_post->getPage().getPageId();
        url = m_post->getPage().getPicBig();
    }

    UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
    QPixmap *p = cache->getPixmap(uid, UTIL::FbUserPicCache::PicBig,
                               url);

    if (p == 0)
    {
        QNetworkRequest nr;
        nr.setUrl(url);
        m_nam->get(nr);
    }
    else
    {
        QLabel *l = new QLabel();
        l->setPixmap(*p);
        ((QHBoxLayout *)layout())->insertWidget(0,l,0,Qt::AlignTop);
    }
}

void StreamPostWidget::gotPosterPixmap(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);

        QString uid;
        QUrl url;

        if (m_post->isFromUser())
        {
            uid = m_post->getPoster().getUID();
            url = m_post->getPoster().getPicBig();
        }
        else
        {
            uid = m_post->getPage().getPageId();
            url = m_post->getPage().getPicBig();
        }

        UTIL::FbUserPicCache *cache = UTIL::FbUserPicCache::getInstance();
        cache->cachePixmap(uid, UTIL::FbUserPicCache::PicBig,
                           url, p);

        ((QHBoxLayout *)layout())->insertWidget(0,l,0,Qt::AlignTop);
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }




}

void StreamPostWidget::scrollToBottom() {

}

void StreamPostWidget::closeEvent(QCloseEvent *event) {
    emit closed(this);
    event->accept();
}

} // namespace GUI
