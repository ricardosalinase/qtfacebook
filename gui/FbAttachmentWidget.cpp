#include "FbAttachmentWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "gui/FbPhotoLabel.h"
#include "util/FbPhotoCache.h"

namespace GUI {

FbAttachmentWidget::FbAttachmentWidget(DATA::FbStreamAttachment *attachment, QWidget *parent) :
    QWidget(parent)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));


    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;


    QList<DATA::FbStreamMedia *> mList = attachment->getMedia();
    int numMedia = mList.size();

    if (numMedia != 0)
    {
        hLayout = new QHBoxLayout();

        // 9 times out of 10 if there's multiple media objects in the attachment
        // it's going to be a Facebook photo album post, but technically
        // you can send multiples of anything. The only way to actually tell is by
        // looking at the <type> (FbStreamMedia.getType()) inside each object.
        // If they're mixed types, AMFYOYO. The <fb_object_type> of the attachment
        // itself is pretty useless and inconsistant at best. This wouldn't be a
        // problem except that to get at facebook photos and albums, you have
        // to get at the <pid> and <aid> contained in a <photo> object that
        // is inside the media object if it is a Facebook photo/album post.

        for (int i = 0; i < mList.size(); i++)
        {
            DATA::FbStreamMedia *sm = mList.at(i);

            if (sm->getType() == "photo")
            {
                UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

                QString pid = sm->getMediaDetail().value("pid");
                //QString aid = sm->getMediaDetail().value("aid");
                GUI::FbPhotoLabel *l = new GUI::FbPhotoLabel("pid:" + pid);
                connect(l, SIGNAL(userClickedImage(QString)),
                        this, SIGNAL(userClickedImage(QString)));

                QPixmap *p = cache->getPixmap(pid, UTIL::FbPhotoCache::Pic,
                                              sm->getSrc());

                if (p == 0)
                {
                    QNetworkRequest nr;
                    nr.setUrl(sm->getSrc());
                    QNetworkReply *reply = m_nam->get(nr);
                    QPair<RequestType, GUI::FbPhotoLabel *> pair(Photo, l);
                    m_outstandingRequests.insert(reply, pair);
                }
                else
                {
                    l->setPixmap(*p);
                    l->setMinimumHeight(p->height());
                    delete p;
                }

                hLayout->addWidget(l,0,Qt::AlignTop);
            }
            // else if ... // need to add link, mp3, flash, etc handlers.
            else
            {
                GUI::FbPhotoLabel *l = new GUI::FbPhotoLabel(sm->getHref().toString());
                connect(l, SIGNAL(userClickedImage(QString)),
                        this, SIGNAL(userClickedImage(QString)));
                QUrl url(sm->getSrc());
                QNetworkRequest nr;
                nr.setUrl(url);
                QNetworkReply *reply = m_nam->get(nr);
                QPair<RequestType, GUI::FbPhotoLabel *>  pair(Other, l);
                m_outstandingRequests.insert(reply,pair);
                hLayout->addWidget(l,0,Qt::AlignTop);

            }
        }

    }

    // Now that our images are on their way, lets lay out the text
    vLayout = new QVBoxLayout();

    if (attachment->getName() != "")
    {
        QLabel *name = new QLabel("<a href=\"" + attachment->getHref().toString() + "\">" +
                              attachment->getName() + "</a>");

        name->setWordWrap(true);
        vLayout->addWidget(name,0,Qt::AlignTop);

    }

    if (attachment->getCaption() != "")
    {
        QLabel *caption = new QLabel(attachment->getCaption());
        caption->setWordWrap(true);
        vLayout->addWidget(caption);
    }

    if (attachment->getDescription() != "")
    {
        QLabel *description = new QLabel(attachment->getDescription());
        description->setWordWrap(true);
        vLayout->addWidget(description,1);
    }

    QList<DATA::FbStreamAttachmentProperty *> pList = attachment->getProperties();

    for (int i = 0; i < pList.size(); i++)
    {
        vLayout->addWidget(new QLabel(pList.at(i)->getName() + ": " +
                                             pList.at(i)->getText()));
    }

    vLayout->addStretch();

    if (numMedia == 1) // this layout goes next to the image
    {
        hLayout->addLayout(vLayout,1);
        setLayout(hLayout);
    }
    else if (numMedia > 1) // It goes below it
    {
        vLayout->insertLayout(0,hLayout);
        setLayout(vLayout);
    }
    else // We have no hLayout - no media images
    {
        setLayout(vLayout);
    }

}

void FbAttachmentWidget::gotNetworkReply(QNetworkReply *reply) {
    QPair<RequestType, GUI::FbPhotoLabel *> pair = m_outstandingRequests.take(reply);
    GUI::FbPhotoLabel *pl = pair.second;
    RequestType type = pair.first;

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        pl->setPixmap(p);
        pl->setMinimumHeight(p.height());

        if (type == Photo)
        {
            UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();
            QString pid = pl->getPid().split(":").at(1);
            cache->cachePixmap(pid, UTIL::FbPhotoCache::Pic,
                               reply->request().url(), p);
        }

    }
    else
    {
        qDebug() << "FbAttachmentWidget::gotNetworkReply(); QNetworkReply error: " << reply->errorString();
    }

    reply->deleteLater();

}

QSize FbAttachmentWidget::minimumSizeHint () const {
    if (sizeHint().width() > 450)
        return sizeHint();
    else return QSize(450,sizeHint().height());
}

} // namespace GUI
