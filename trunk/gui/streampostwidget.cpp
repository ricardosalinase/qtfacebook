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
    m_triedBothIcons(false),
    m_userInfo(info)
{

    m_factory = new API::Factory(info);
    connect(m_factory, SIGNAL(apiStreamAddComment(API::Stream::AddComment*)),
           this, SLOT(apiStreamAddComment(API::Stream::AddComment*)));
    connect(m_factory, SIGNAL(apiStreamAddCommentFailed(API::Stream::AddComment*)),
            this, SLOT(apiStreamAddCommentFailed(API::Stream::AddComment*)));
    connect(m_factory, SIGNAL(apiFqlGetComments(API::FQL::GetComments*)),
            this, SLOT(gotComments(API::FQL::GetComments*)));
    connect(m_factory, SIGNAL(apiFqlGetCommentsFailed(API::FQL::GetComments*)),
            this, SLOT(getCommentsFailed(API::FQL::GetComments*)));
    connect(m_factory, SIGNAL(apiStreamRemoveComment(API::Stream::RemoveComment*)),
            this, SLOT(apiStreamRemoveComment(API::Stream::RemoveComment*)));
    connect(m_factory, SIGNAL(apiStreamRemoveCommentFailed(API::Stream::RemoveComment*)),
            this, SLOT(apiStreamRemoveCommentFailed(API::Stream::RemoveComment*)));

    m_commentTimer = new QTimer();
    connect(m_commentTimer, SIGNAL(timeout()),
            this, SLOT(getComments()));

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPosterPixmap(QNetworkReply*)));
    m_nam2 = new QNetworkAccessManager(this);
    connect(m_nam2,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPhoto(QNetworkReply*)));

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
    m_contentLayout->addSpacing(10);




    m_commentContainer = new QWidget();
    m_commentScrollArea = new QScrollArea();

    m_commentLayout = new QVBoxLayout();

    m_commentLayout->addStretch();

    m_commentContainer->setLayout(m_commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);
    m_commentScrollArea->setWidgetResizable(true);
    m_commentScrollArea->setVisible(false);
    m_commentScrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    m_contentLayout->addWidget(m_commentScrollArea,2);


    m_progressWidget = new QWidget();
    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->setMargin(0);
    m_commentProgressBar = new QProgressBar();
    m_commentProgressBar->setMinimum(0);
    m_commentProgressBar->setMaximum(0);
    m_commentProgressBar->setMaximumHeight(10);

    QLabel *l = new QLabel("Updating Comments");
    l->setStyleSheet("font-size: 8pt;");
    hbl->addWidget(l);
    hbl->addWidget(m_commentProgressBar);
    m_progressWidget->setLayout(hbl);
    //m_contentLayout->addWidget(m_progressWidget);
    QHBoxLayout *hbl2 = new QHBoxLayout();
    l = new QLabel();
    l->setMinimumHeight(15);
    hbl2->addWidget(l);
    hbl2->addWidget(m_progressWidget);
    m_contentLayout->addLayout(hbl2);

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
    m_contentLayout->addSpacing(5);
    m_contentLayout->addWidget(m_commentEdit,0);

    m_addCommentButton = new QPushButton("Add Comment");
    m_contentLayout->addWidget(m_addCommentButton, 0);
    connect(m_addCommentButton, SIGNAL(clicked()),
            this, SLOT(commentButtonClicked()));


    mainLayout->insertLayout(2,m_contentLayout,1);
    setLayout(mainLayout);

    //this->resize(800,600);
    getPosterPixmap();
    getComments();

}

StreamPostWidget::~StreamPostWidget() {

    m_commentTimer->stop();
    delete m_commentTimer;

    while (!m_post->getCommentList()->empty())
    {
        DATA::StreamComment *c = m_post->getCommentList()->takeFirst();
        delete c;
    }

}

void StreamPostWidget::getComments()
{
    m_progressWidget->setVisible(true);
    API::Method *method = m_factory->createMethod("fql.multiquery.getComments");

    method->setArgument("post_id", m_post->getPostId());
    bool rc = method->execute();
    if (!rc)
    {
        qDebug() << "fql.multiquery.getComments failed to execute: " << method->errorString();
        delete method;
    }


}

void StreamPostWidget::gotComments(API::FQL::GetComments *method) {

    QList<DATA::StreamComment *> cList = method->getCommentList();
    int num = cList.size();

    // TODO: Should be only get the comments we don't have?

    // This is the *only* way I've found to make this work
    // otherwise the viewport of the QScrollArea doesn't repaint
    // and you end up with artifacts of old comments
    m_commentScrollArea->takeWidget();
    delete m_commentContainer;
    while (!m_post->getCommentList()->empty())
    {
        DATA::StreamComment *c = m_post->getCommentList()->takeFirst();
        delete c;
    }

    m_commentContainer = new QWidget();
    m_commentLayout = new QVBoxLayout();

    CommentWidget *last = 0;
    while(!cList.empty())
    {
        DATA::StreamComment *c = cList.takeFirst();
        m_post->getCommentList()->append(c);

        bool canDelete = false;
        if(m_post->getCommentList()->canRemove() ||
            c->getFromId() == m_userInfo->getUID())
        {
            canDelete = true;
        }

        CommentWidget *cw = new CommentWidget(c, canDelete);
        m_commentLayout->addWidget(cw);
        connect(cw, SIGNAL(userClickedDelete(GUI::CommentWidget*)),
                this, SLOT(userDeletedComment(GUI::CommentWidget*)));
        last = cw;
    }
    m_commentLayout->addStretch();
    m_commentContainer->setLayout(m_commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);


    if (num)
    {
        m_commentScrollArea->setVisible(true);
        m_commentScrollArea->ensureWidgetVisible(last);
        gotContentUpdate();
    }
    else
    {
        m_commentScrollArea->setVisible(false);
    }

    delete method;
    m_progressWidget->setVisible(false);
    gotContentUpdate();
    m_commentTimer->start(60000);
}

void StreamPostWidget::getCommentsFailed(API::FQL::GetComments *method) {

    qDebug() << "API::FQL::GetComments Failed. Retrying in 5 seconds";
    m_commentTimer->start(5000);
    m_progressWidget->setVisible(false);
    delete method;
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

    int h;
    int w;

    size().height() < sizeHint().height() ? h = sizeHint().height() : h = size().height();
    size().width() < sizeHint().width() ? w = sizeHint().width() : w = size().width();

    resize(w,h);


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

void StreamPostWidget::userDeletedComment(GUI::CommentWidget *commentWidget) {

    qDebug() << "Delete comment: " << commentWidget->getComment()->getCommentId();

    API::Method *method = m_factory->createMethod("stream.removeComment");

    method->setArgument("comment_id",commentWidget->getComment()->getCommentId());
    if(!method->execute())
    {
        qDebug() << "API::Stream::removeComment failed to execute: " << method->getErrorStr();
        delete method;
    }

}

void StreamPostWidget::apiStreamAddComment(API::Stream::AddComment *method) {
    qDebug() << "Added Comment";
    m_commentEdit->clear();
    m_addCommentButton->setEnabled(true);
    delete method;
    getComments();

}

void StreamPostWidget::apiStreamAddCommentFailed(API::Stream::AddComment *method) {
    qDebug() << "Failed to add Comment";
    m_addCommentButton->setEnabled(true);
    delete method;
}

void StreamPostWidget::apiStreamRemoveComment(API::Stream::RemoveComment *method) {

    qDebug() << "Removed Comment";
    delete method;
    getComments();

}

void StreamPostWidget::apiStreamRemoveCommentFailed(API::Stream::RemoveComment *method) {
    qDebug() << "Failed to remove comment";
    delete method;
}

} // namespace GUI
