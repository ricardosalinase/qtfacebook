#include "FbPhotoViewWidget.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>

#include "FbCommentManager.h"
#include "util/FbPhotoCache.h"

namespace GUI {

FbPhotoViewWidget::FbPhotoViewWidget(DATA::FbPhoto *photo, UserInfo *info, QWidget *parent) :
    QWidget(parent),
    m_photo(photo),
    m_info(info),
    m_isOwner(false)
{
    resize(620,620);
    this->setStyleSheet("background : white;");
    if (m_info->getUID() == photo->getOwnerId())
        m_isOwner = true;

    if (photo->isUserOwned())
        this->setWindowTitle(photo->getUserInfo().getName());
    else
        this->setWindowTitle(photo->getPageInfo().getName());

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QString s = "<style type=\"text/css\">a { text-decoration: none; }</style>"
                "From " + this->windowTitle() + "'s album <a href=\"aid:" +
                photo->getAlbumId() + "\">" + photo->getAlbumName() +
                "</a>";

    QLabel *l = new QLabel(s);
    connect(l, SIGNAL(linkActivated(QString)),
            this, SIGNAL(userClickedLink(QString)));
    mainLayout->addWidget(l);




    UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

    QPixmap *p = cache->getPixmap(m_photo->getPhotoId(), UTIL::FbPhotoCache::PicBig,
                                  photo->getSrcBig());

    if (p == 0)
    {
        m_progress = new LoadingProgressWidget("Loading Photo");
        mainLayout->addWidget(m_progress, 1, Qt::AlignCenter);
        QNetworkRequest nr;
        nr.setUrl(photo->getSrcBig());
        m_nam->get(nr);
    }
    else
    {
        QLabel *l = new QLabel();
        l->setPixmap(*p);
        l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        mainLayout->addWidget(l);
        delete p;
    }

    if (photo->getCaption() != "")
    {
        l = new QLabel("\"" + photo->getCaption() + "\"");
        mainLayout->addWidget(l);
    }

    FbCommentManager *cm = new FbCommentManager(m_photo->getObjectId(),
                                                FbCommentManager::ObjectId,
                                                m_isOwner,
                                                m_info);
    mainLayout->addWidget(cm);

    setLayout(mainLayout);

}

void FbPhotoViewWidget::gotNetworkReply(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);
        l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        ((QVBoxLayout*)layout())->removeWidget(m_progress);
        delete m_progress;
        ((QVBoxLayout*)layout())->insertWidget(1,l);

        UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();
        cache->cachePixmap(m_photo->getPhotoId(), UTIL::FbPhotoCache::PicBig,
                           m_photo->getSrcBig(), p);

    }
    else
    {
        qDebug() << "Error retrieving photo: " << reply->errorString();
    }

    reply->deleteLater();

}

void FbPhotoViewWidget::closeEvent(QCloseEvent *event) {
    emit closed(this);
    event->accept();
}

} // namespace GUI
