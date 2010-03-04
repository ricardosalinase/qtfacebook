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

    if (m_info->getUID() == photo->getOwnerId())
        m_isOwner = true;

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    FbCommentManager *cm = new FbCommentManager(m_photo->getPhotoId(),
                                                FbCommentManager::PostId,
                                                m_isOwner,
                                                m_info);
    mainLayout->addWidget(cm);

    setLayout(mainLayout);

    UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

    QPixmap *p = cache->getPixmap(m_photo->getPhotoId(), UTIL::FbPhotoCache::PicBig,
                                  photo->getSrcBig());

    if (p == 0)
    {
        QNetworkRequest nr;
        nr.setUrl(photo->getSrcBig());
        m_nam->get(nr);
    }
    else
    {
        QLabel *l = new QLabel();
        l->setPixmap(*p);
        l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        ((QVBoxLayout*)layout())->insertWidget(0,l);
        delete p;
    }

}

void FbPhotoViewWidget::gotNetworkReply(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);
        l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        ((QVBoxLayout*)layout())->insertWidget(0,l);

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
