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
#include "gui/FbStreamPostPhotoWidget.h"


namespace GUI {

StreamPostWidget::StreamPostWidget(DATA::StreamPost *post, QWidget *parent) :
    QWidget(parent),
    m_post(post),
    m_triedBothIcons(false)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPosterPixmap(QNetworkReply*)));
    m_nam2 = new QNetworkAccessManager(this);
    connect(m_nam2,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPhoto(QNetworkReply*)));

    this->setStyleSheet("background : white;");

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_contentLayout = new QVBoxLayout();
    m_contentLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    if (post->isFromUser())
        this->setWindowTitle(post->getPoster().getName());
    else
        this->setWindowTitle(post->getPage().getName());

    if (post->getMessage() != "")
    {
        QLabel *message = new QLabel(post->getMessage());
        message->setWordWrap(true);
        message->setMinimumWidth(500);
        m_contentLayout->addWidget(message,0,Qt::AlignTop);
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

    // Get the app icon, prefer the one in post if present
    if (m_post->getAppIcon().toString() != "")
    {
        m_triedBothIcons = true;
        QNetworkRequest nr;
        nr.setUrl(m_post->getAppIcon());
        QNetworkReply *reply2 = m_nam2->get(nr);
        m_outstandingNetworkRequests.insert(reply2, AppIcon);
    }
    else if (post->getAttachment()->getIcon().toString() != "")
    {
        m_triedBothIcons = true;
        QNetworkRequest nr;
        nr.setUrl(post->getAttachment()->getIcon());
        QNetworkReply *reply = m_nam2->get(nr);
        m_outstandingNetworkRequests.insert(reply, AppIcon);
    }


    if (!post->getAttachment()->isEmpty())
    {
        DATA::FbStreamAttachment *attachment = post->getAttachment();
        QString type = attachment->getFbObjectType();

        if (type == "album" || type == "photo")
        {
            GUI::FbStreamPostPhotoWidget *pw = new GUI::FbStreamPostPhotoWidget(attachment, this);
            m_contentLayout->addWidget(pw);
        }
        else if (type == "") // No FbObjectType specified in the attachment
        {

            m_linkLayout = new QHBoxLayout();
            m_linkLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
            QVBoxLayout *linkTextLayout = new QVBoxLayout();
            linkTextLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
            if (attachment->getName() != "")
            {
                QLabel *name = new QLabel("<a href=\"" + attachment->getHref().toString() + "\">" +
                                      attachment->getName() + "</a>");
                name->setMinimumWidth(500);
                name->setWordWrap(true);
                linkTextLayout->addWidget(name,0,Qt::AlignTop);
            }

            if (attachment->getCaption() != "")
            {
                QLabel *caption = new QLabel(attachment->getCaption());
                caption->setWordWrap(true);
                linkTextLayout->addWidget(caption);
            }

            if (attachment->getDescription() != "")
            {
                QLabel *description = new QLabel(attachment->getDescription());
                description->setWordWrap(true);
                linkTextLayout->addWidget(description);
            }

            QList<DATA::FbStreamAttachmentProperty *> pList = attachment->getProperties();

            for (int i = 0; i < pList.size(); i++)
            {
                linkTextLayout->addWidget(new QLabel(pList.at(i)->getName() + ": " +
                                                     pList.at(i)->getText()));
            }

            //linkTextLayout->addStretch();
            m_linkLayout->insertLayout(1,linkTextLayout,0);
            m_contentLayout->addLayout(m_linkLayout,0);


            QList<DATA::FbStreamMedia *> mList = attachment->getMedia();
            for (int i = 0; i < mList.size(); i++)
            {
                if (mList.at(i)->getType() == "link")
                {
                    QNetworkRequest nr;
                    nr.setUrl(mList.at(i)->getSrc());
                    QNetworkReply *reply = m_nam2->get(nr);
                    m_outstandingNetworkRequests.insert(reply, LinkThumb);
                }
            }
        }
    }

    m_contentLayout->addLayout(m_ageLineLayout,0);
    m_contentLayout->addStretch();

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

    
    m_contentLayout->addWidget(m_commentScrollArea,1);
    mainLayout->insertLayout(1,m_contentLayout,1);
    setLayout(mainLayout);

    //this->resize(800,600);
    getPosterPixmap();

}

StreamPostWidget::~StreamPostWidget() {
    delete m_post;
}

void StreamPostWidget::gotPhoto(QNetworkReply *reply) {

    RequestType t = m_outstandingNetworkRequests.take(reply);

    if (reply->error() == QNetworkReply::NoError)
    {
        if (t == Photo)
        {
            QPixmap p;
            p.loadFromData(reply->readAll());
            QLabel *l = new QLabel();
            l->setPixmap(p);
            l->setMinimumHeight(p.height());
            m_photoLayout->insertWidget(0,l);

        }
        else if (t == AppIcon)
        {
            QPixmap p;
            p.loadFromData(reply->readAll());
            QLabel *l = new QLabel();
            l->setPixmap(p);
            m_ageLineLayout->insertWidget(0,l,0);

        } else if (t == LinkThumb)
        {
            QPixmap p;
            p.loadFromData(reply->readAll());
            QLabel *l = new QLabel();
            l->setPixmap(p);
            m_linkLayout->insertWidget(0,l,0,Qt::AlignTop);
            updateGeometry();
        }
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toEncoded();

        if (t == AppIcon && !m_triedBothIcons)
        {
            m_triedBothIcons = true;
            if (m_post->hasAttachment() && !m_post->getAttachment()->isEmpty()
                            && m_post->getAttachment()->getIcon().toString() != "")
            {
                QNetworkRequest nr;
                nr.setUrl(m_post->getAttachment()->getIcon());
                QNetworkReply *reply2 = m_nam2->get(nr);
                m_outstandingNetworkRequests.insert(reply2, AppIcon);
            }

        }

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
        l->setMinimumWidth(p.width());

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
