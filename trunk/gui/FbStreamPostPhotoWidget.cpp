#include "FbStreamPostPhotoWidget.h"
#include <QLabel>
#include <QDebug>
#include <QNetworkRequest>
#include <QVariant>

#include "util/FbPhotoCache.h"
#include "data/FbPhoto.h"
#include "gui/FbPhotoLabel.h"

namespace GUI {

FbStreamPostPhotoWidget::FbStreamPostPhotoWidget(DATA::FbStreamAttachment *attachment, QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(450);
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    m_mainLayout = new QVBoxLayout();
    m_photoLayout = new QHBoxLayout();
    m_photoLayout->setSpacing(20);
    m_photoLayout->addStretch();
    m_mainLayout->addLayout(m_photoLayout);
    m_mainLayout->setMargin(0);
    QList<DATA::FbStreamMedia *> mList = attachment->getMedia();

    UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

    // I actually don't like this at all. When you get a post from the
    // stream table, it may have an attachment. That attachment contains
    // one or more <stream_media> objects. Those objects are composites,
    // containing info from other tables, but not in a well formed manor.
    // In the case of photos, you end up with some of the information from
    // the photo table in the <stream_media> section with different names,
    // then also get another object called <photo> with the aid, pid, and
    // owner from the actual photo table. I'm using our DATA::FbPhoto object
    // here to encapsulate that data, but it's useless outside of this
    // context because another FQL query is required to fill it out completely.
    // We could do that here, but it'd be a waste to do that for every photo as
    // the user may not even want to see it outside of this context, and because
    // of the rate limiting on FQL queries, we want to do as few as possible.
    for (int i = 0; i < mList.size(); i++)
    {
        DATA::FbStreamMedia *streamMedia = mList.at(i);
        DATA::FbPhoto currentPhoto;
        QVariant *mediaVariant = (streamMedia->getMedia());
        if (mediaVariant->canConvert<DATA::FbPhoto>())
        {
            currentPhoto = mediaVariant->value<DATA::FbPhoto>();
            QString srcString = streamMedia->getSrc().toString();
            currentPhoto.setSrc(srcString);
            m_photoMap.insert(currentPhoto.getPhotoId(), currentPhoto);

            QPixmap *p = cache->getPixmap(currentPhoto.getPhotoId(), UTIL::FbPhotoCache::Pic,
                                          streamMedia->getSrc());

            if (p == 0)
            {
                QNetworkRequest nr;
                QUrl url = currentPhoto.getSrc();
                nr.setUrl(url);
                QNetworkReply *reply = m_nam->get(nr);
                m_outstandingNetworkRequests.insert(reply, currentPhoto);
            }
            else
            {
                FbPhotoLabel *l = new FbPhotoLabel(currentPhoto.getPhotoId());
                connect(l,SIGNAL(userClickedImage(QString)),
                        this, SLOT(userClickedImage(QString)));
                l->setPixmap(*p);
                l->setMinimumHeight(p->height());
                m_photoLayout->addWidget(l,0,Qt::AlignTop);
                m_photoLayout->addStretch(1);
                delete p;
            }
        }
    }



    if (attachment->getName() != "")
    {
       m_mainLayout->addWidget(new QLabel("Album: " + attachment->getName()));
    }
    if (attachment->getCaption() != "")
    {
       m_mainLayout->addWidget(new QLabel( attachment->getCaption()));
    }


    setLayout(m_mainLayout);


}

void FbStreamPostPhotoWidget::gotNetworkReply(QNetworkReply *reply) {

    DATA::FbPhoto currentPhoto = m_outstandingNetworkRequests.take(reply);

    if (reply->error() == QNetworkReply::NoError)
    {
        UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

        QPixmap p;
        p.loadFromData(reply->readAll());
        FbPhotoLabel *l = new FbPhotoLabel(currentPhoto.getPhotoId());
        connect(l,SIGNAL(userClickedImage(QString)),
                this, SLOT(userClickedImage(QString)));
        l->setPixmap(p);

        cache->cachePixmap(currentPhoto.getPhotoId(), UTIL::FbPhotoCache::Pic,
                           currentPhoto.getSrc(),p);

        l->setMinimumHeight(p.height());
        m_photoLayout->addWidget(l,0,Qt::AlignTop);
        m_photoLayout->addStretch(1);


    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toEncoded();
    }

    reply->deleteLater();
}

void FbStreamPostPhotoWidget::userClickedImage(QString pid) {

    // Rather than sending out a partial FbPhoto object, we'll send the info
    // that's going to be needed in the next context.
    emit photoSelected(pid, m_photoMap[pid].getAlbumId(),
                       m_photoMap[pid].getOwnerId());

}

} // namespace GUI
