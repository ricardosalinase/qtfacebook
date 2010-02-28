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
#include <QCoreApplication>
#include <QFrame>
#include <QPushButton>

#include "util/agestring.h"
#include "commentwidget.h"
#include "util/fbuserpiccache.h"
#include "data/FbStreamAttachment.h"
#include "gui/FbStreamPostPhotoWidget.h"
#include "gui/FbStreamPostContentWidget.h"
#include "gui/FbCommentManager.h"


namespace GUI {

StreamPostWidget::StreamPostWidget(DATA::StreamPost *post, UserInfo *info, QWidget *parent) :
    QWidget(parent),
    m_post(post),
    m_triedBothIcons(false),
    m_userInfo(info)
{

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    this->setStyleSheet("background : white;");
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QFrame *f = new QFrame();
    f->setMinimumWidth(10);
    f->setFrameShape(QFrame::VLine);
    mainLayout->insertWidget(1,f);
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
        message->setTextInteractionFlags(Qt::TextBrowserInteraction);
        message->setWordWrap(true);
        message->setMinimumWidth(450);
        message->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
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
    if (m_post->getAppInfo().getIconUrl() != "")
    {
        m_triedBothIcons = true;
        QNetworkRequest nr;
        nr.setUrl(QUrl(m_post->getAppInfo().getIconUrl()));
        QNetworkReply *reply = m_nam->get(nr);
        m_outstandingNetworkRequests.insert(reply, AppIcon);
    }
    else if (post->getAttachment()->getIcon().toString() != "")
    {
        m_triedBothIcons = true;
        QNetworkRequest nr;
        nr.setUrl(post->getAttachment()->getIcon());
        QNetworkReply *reply = m_nam->get(nr);
        m_outstandingNetworkRequests.insert(reply, AppIcon);
    }


    if (!post->getAttachment()->isEmpty())
    {
        DATA::FbStreamAttachment *attachment = post->getAttachment();
        QString type = attachment->getFbObjectType();

        if (type == "album" || type == "photo")
        {
            GUI::FbStreamPostPhotoWidget *pw = new GUI::FbStreamPostPhotoWidget(attachment);
            connect(pw, SIGNAL(photoSelected(QString,QString,QString)),
                    this, SIGNAL(photoSelectedFromStreamPost(QString,QString,QString)));
            pw->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
            m_contentLayout->addSpacing(10);
            m_contentLayout->addWidget(pw);

        }
        //else if (type == "event") {}
        else // if (type == "") // No FbObjectType specified in the attachment
        {
            GUI::FbStreamPostContentWidget *cw = new GUI::FbStreamPostContentWidget(attachment);
            cw->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
            connect(cw, SIGNAL(contentLoaded()),
                    this, SLOT(gotContentUpdate()));
            m_contentLayout->addSpacing(10);
            m_contentLayout->addWidget(cw);
        }
    }

    m_contentLayout->addLayout(m_ageLineLayout,0);
    m_contentLayout->addStretch();
    m_contentLayout->addSpacing(10);


    GUI::FbCommentManager *manager = new FbCommentManager(m_post->getPostId(),
                                                          FbCommentManager::PostId,
                                                          m_post->getCommentList()->canRemove(),
                                                          m_userInfo);
    m_contentLayout->addWidget(manager,2);


    mainLayout->insertLayout(2,m_contentLayout,1);
    setLayout(mainLayout);


    getPosterPixmap();


}

StreamPostWidget::~StreamPostWidget() {


    // Also deletes any outstanding network requests.
    delete m_nam;


}

void StreamPostWidget::gotNetworkReply(QNetworkReply *reply) {

    RequestType t = m_outstandingNetworkRequests.take(reply);

    if (reply->error() == QNetworkReply::NoError)
    {
        if (t == AppIcon)
        {
            QPixmap p;
            p.loadFromData(reply->readAll());
            QLabel *l = new QLabel();
            l->setPixmap(p);
            m_ageLineLayout->insertWidget(0,l,0);

        }
        else if (t == PosterPixmap)
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
            updateGeometry();
            gotContentUpdate();
            update();
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
                QNetworkReply *reply2 = m_nam->get(nr);
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
        QNetworkReply *reply = m_nam->get(nr);
        m_outstandingNetworkRequests.insert(reply, PosterPixmap);
    }
    else
    {
        QLabel *l = new QLabel();
        l->setPixmap(*p);
        delete p;
        ((QHBoxLayout *)layout())->insertWidget(0,l,0,Qt::AlignTop);
        adjustSize();
        update();
    }
}



void StreamPostWidget::closeEvent(QCloseEvent *event) {
    emit closed(this);
    event->accept();
}

void StreamPostWidget::gotContentUpdate() {

    QCoreApplication::sendPostedEvents();

    int h;
    int w;

    size().height() < sizeHint().height() ? h = sizeHint().height() : h = size().height();
    size().width() < sizeHint().width() ? w = sizeHint().width() : w = size().width();

    resize(w,h);


}

} // namespace GUI
