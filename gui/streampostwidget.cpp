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
#include <QDesktopWidget>
#include <QApplication>

#include "util/agestring.h"
#include "commentwidget.h"
#include "util/fbuserpiccache.h"
#include "data/FbStreamAttachment.h"
#include "gui/FbCommentManager.h"
#include "gui/FbLikeManager.h"
#include "gui/FbAttachmentWidget.h"
#include "util/HyperLink.h"


namespace GUI {

StreamPostWidget::StreamPostWidget(DATA::StreamPost *post, QWidget *parent) :
    QWidget(parent),
    m_post(post),
    m_triedBothIcons(false)
{



    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    //this->setStyleSheet("background : white;");
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    setPalette(palette);


    QHBoxLayout *mainLayout = new QHBoxLayout();
    QFrame *f = new QFrame();
    f->setMinimumWidth(10);
    f->setFrameShape(QFrame::VLine);
    mainLayout->insertWidget(1,f);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    m_contentLayout = new QVBoxLayout();
    m_contentLayout->setSizeConstraint(QLayout::SetMinimumSize);

    if (post->isFromUser())
        this->setWindowTitle(post->getPoster().getName());
    else
        this->setWindowTitle(post->getPage().getName());

    if (post->getMessage() != "" || (post->getTargetId().compare("") != 0))
    {
        QLabel *message = new QLabel();
        QString text = post->getMessage();
        if (post->getTargetId().compare("") != 0)
            text.prepend("-> " + post->getTarget().getName() + ": ");
        message->setText(UTIL::hyperLink(text));
        message->setTextInteractionFlags(Qt::TextBrowserInteraction);
        message->setWordWrap(true);
        message->setMinimumWidth(450);
        message->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        connect(message, SIGNAL(linkActivated(QString)),
                this, SIGNAL(contentClicked(QString)));
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

    // Get the app icon, prefer the one in attachment if present
    if (post->getAttachment()->getIcon().toString() != "")
    {
        QNetworkRequest nr;
        nr.setUrl(post->getAttachment()->getIcon());
        QNetworkReply *reply = m_nam->get(nr);
        m_outstandingNetworkRequests.insert(reply, AppIcon);
    }
    else if (m_post->getAppInfo().getIconUrl() != "")
    {
        m_triedBothIcons = true;
        QNetworkRequest nr;
        nr.setUrl(QUrl(m_post->getAppInfo().getIconUrl()));
        QNetworkReply *reply = m_nam->get(nr);
        m_outstandingNetworkRequests.insert(reply, AppIcon);
    }



    if (!post->getAttachment()->isEmpty())
    {
        DATA::FbStreamAttachment *attachment = post->getAttachment();
        FbAttachmentWidget *aw = new FbAttachmentWidget(attachment);
        aw->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        connect(aw, SIGNAL(userClickedUrl(QString)),
                this, SIGNAL(contentClicked(QString)));
        m_contentLayout->addWidget(aw);
    }

    m_contentLayout->addLayout(m_ageLineLayout,0);
    m_contentLayout->addStretch();
    m_contentLayout->addSpacing(10);


    GUI::FbCommentManager *manager = new FbCommentManager(m_post->getPostId(),
                                                          FbCommentManager::PostId,
                                                          m_post->getCommentList()->canRemove());
    manager->setMaximumHeight(400);

    QString fbObjectId = m_post->getPostId();
    GUI::FbLikeManager *likeManager = new FbLikeManager(fbObjectId, m_post->userLikes());
    connect(likeManager, SIGNAL(userChangedLike(bool)),
            this, SLOT(likeChanged(bool)));

    QHBoxLayout *hLayout = new QHBoxLayout();
    m_showAddCommentButton = new QToolButton();
    m_showAddCommentButton->setAutoFillBackground(true);
    palette = m_showAddCommentButton->palette();
    palette.setColor(QPalette::Button,QColor(82,110,166));
    m_showAddCommentButton->setPalette(palette);
    m_showAddCommentButton->setIcon(QIcon(":/uiImages/addComment_50_50.jpg"));
    m_showAddCommentButton->setIconSize(QSize(40,40));
    m_showAddCommentButton->setToolTip("Comment");
    connect(m_showAddCommentButton, SIGNAL(clicked()),
            manager, SLOT(showAddComment()));
    hLayout->addWidget(m_showAddCommentButton,0);

    m_likeButton = new QToolButton();
    this->likeChanged(m_post->userLikes());
    m_likeButton->setIconSize(QSize(40,40));

    connect(m_likeButton, SIGNAL(clicked()),
            likeManager, SLOT(toggleUserLikes()));
    hLayout->addWidget(m_likeButton,0);
    hLayout->addStretch(1);

    m_contentLayout->addLayout(hLayout,0);
    m_contentLayout->addWidget(likeManager,0);
    m_contentLayout->addWidget(manager,2);

    //manager->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    mainLayout->insertLayout(2,m_contentLayout,1);
    setLayout(mainLayout);


    getPosterPixmap();

    QDesktopWidget *dw = QApplication::desktop();
    QRect r = dw->screenGeometry();
    move(r.width() - 240, 40);


}

StreamPostWidget::~StreamPostWidget() {

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
            if (m_post->getAppInfo().getIconUrl() != "")
            {
                QNetworkRequest nr;
                nr.setUrl(QUrl(m_post->getAppInfo().getIconUrl()));
                QNetworkReply *reply2 = m_nam->get(nr);
                m_outstandingNetworkRequests.insert(reply2, AppIcon);
            }

        }

    }

    reply->deleteLater();

}

void StreamPostWidget::likeChanged(bool likes)
{
    m_post->userLikes(likes);
    if (m_post->userLikes())
    {
        m_likeButton->setIcon(QIcon(":/uiImages/thumbDown_50_50.jpg"));
        m_likeButton->setToolTip("Stop Liking");
    }
    else
    {
        m_likeButton->setIcon(QIcon(":/uiImages/thumbUp_50_50.jpg"));
        m_likeButton->setToolTip("Like");
    }
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
