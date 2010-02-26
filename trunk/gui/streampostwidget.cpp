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


namespace GUI {

StreamPostWidget::StreamPostWidget(DATA::StreamPost *post, UserInfo *info, QWidget *parent) :
    QWidget(parent),
    m_post(post),
    m_triedBothIcons(false)
{

    m_factory = new API::Factory(info);
    connect(m_factory, SIGNAL(apiStreamAddComment(API::Stream::AddComment*)),
           this, SLOT(apiStreamAddComment(API::Stream::AddComment*)));
    connect(m_factory, SIGNAL(apiStreamAddCommentFailed(API::Stream::AddComment*)),
            this, SLOT(apiStreamAddCommentFailed(API::Stream::AddComment*)));


    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPosterPixmap(QNetworkReply*)));
    m_nam2 = new QNetworkAccessManager(this);
    connect(m_nam2,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPhoto(QNetworkReply*)));

    this->setStyleSheet("background : white;");
    QHBoxLayout *mainLayout = new QHBoxLayout();
    //mainLayout->insertSpacing(1,10);
    QFrame *f = new QFrame();
    f->setFrameShape(QFrame::VLine);
    mainLayout->insertWidget(1,f);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_contentLayout = new QVBoxLayout();
    //m_contentLayout->addStrut(500);
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
            GUI::FbStreamPostPhotoWidget *pw = new GUI::FbStreamPostPhotoWidget(attachment);
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
    m_contentLayout->addSpacing(15);
    if (!cList->size())
        m_commentScrollArea->setVisible(false);
    m_commentScrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    m_contentLayout->addWidget(m_commentScrollArea,2);


    m_commentEdit = new QTextEdit();
//    QPalette p = m_commentEdit->palette();
//    p.setColor(QPalette::Base, QColor(240, 240, 255));
//    m_commentEdit->setPalette(p);
    m_commentEdit->setStyleSheet("background-color : #e4e8f8");
    //m_commentEdit->setLineWidth(3);
    m_commentEdit->setAcceptRichText(false);
    m_commentEdit->setFrameStyle(QFrame::Panel);
    m_commentEdit->setFrameShadow(QFrame::Sunken);
    m_commentEdit->setMaximumHeight(75);
    m_commentEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_contentLayout->addSpacing(15);
    m_contentLayout->addWidget(m_commentEdit,0);

    m_addCommentButton = new QPushButton("Add Comment");
    m_contentLayout->addWidget(m_addCommentButton, 0);
    connect(m_addCommentButton, SIGNAL(clicked()),
            this, SLOT(commentButtonClicked()));


    mainLayout->insertLayout(2,m_contentLayout,1);
    setLayout(mainLayout);

    //this->resize(800,600);
    getPosterPixmap();

}

StreamPostWidget::~StreamPostWidget() {

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
        adjustSize();
        update();
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
        updateGeometry();
        gotContentUpdate();
        update();
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

void StreamPostWidget::gotContentUpdate() {

    QCoreApplication::sendPostedEvents();
    resize(sizeHint());


}

void StreamPostWidget::commentButtonClicked() {

    m_addCommentButton->setDisabled(true);

    // See if there's actually a comment
    QString comment = m_commentEdit->toPlainText();
    if (comment != "")
    {
        API::Method *method = m_factory->createMethod("stream.addComment");
        method->setArgument("post_id", m_post->getPostId());
        method->setArgument("comment", comment);
        method->execute();
    }
    else
    {
        m_addCommentButton->setEnabled(true);
    }
}

void StreamPostWidget::apiStreamAddComment(API::Stream::AddComment *method) {
    qDebug() << "Added Comment";
    m_commentEdit->clear();
    m_addCommentButton->setEnabled(true);

}

void StreamPostWidget::apiStreamAddCommentFailed(API::Stream::AddComment *method) {
    qDebug() << "Failed to add Comment";
    m_addCommentButton->setEnabled(true);
}

} // namespace GUI
