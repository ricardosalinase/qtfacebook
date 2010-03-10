#include "FbAttachmentWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "gui/ImageLabel.h"
#include "util/FbPhotoCache.h"

namespace GUI {

FbAttachmentWidget::FbAttachmentWidget(DATA::FbStreamAttachment *attachment, QWidget *parent) :
    QWidget(parent)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    //this->setStyleSheet("background-color : #fff8da");
    this->setAutoFillBackground(true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(255,255,244));
    setPalette(palette);


    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;


    QList<DATA::FbStreamMedia *> mList = attachment->getMedia();
    int numMedia = mList.size();

    // See hack description below where this is used.
    QString savedAlbumId = "";

    vLayout = new QVBoxLayout();

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
                savedAlbumId = sm->getMediaDetail().value("aid");
                GUI::ImageLabel *l = new GUI::ImageLabel("pid:" + pid);
                connect(l, SIGNAL(userClickedImage(QString)),
                        this, SIGNAL(userClickedUrl(QString)));

                QPixmap *p = cache->getPixmap(pid, UTIL::FbPhotoCache::Pic,
                                              sm->getSrc());

                if (p == 0)
                {
                    QNetworkRequest nr;
                    nr.setUrl(sm->getSrc());
                    QNetworkReply *reply = m_nam->get(nr);
                    QPair<RequestType, GUI::ImageLabel *> pair(Photo, l);
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
            else if (sm->getType() == "video")
            {
                m_videoLabel = new GUI::ImageLabel(sm->getMediaDetail().value("source_url"));
                connect(m_videoLabel, SIGNAL(userClickedImage(QString)),
                        this, SLOT(userClickedVideo(QString)));
                QUrl url(sm->getSrc());
                QNetworkRequest nr;
                nr.setUrl(url);
                QNetworkReply *reply = m_nam->get(nr);
                QPair<RequestType, GUI::ImageLabel *>  pair(Other, m_videoLabel);
                m_outstandingRequests.insert(reply,pair);
                hLayout->addWidget(m_videoLabel,0,Qt::AlignTop);

                m_webView = new QWebView();
                m_webView->page()->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
                m_webView->setVisible(false);
                m_webView->setMaximumWidth(400);
                m_webView->setMaximumHeight(300);
                m_webView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
                vLayout->addWidget(m_webView);

            }
            else
            {
                GUI::ImageLabel *l = new GUI::ImageLabel(sm->getHref().toString());
                connect(l, SIGNAL(userClickedImage(QString)),
                        this, SIGNAL(userClickedUrl(QString)));
                QUrl url(sm->getSrc());
                QNetworkRequest nr;
                nr.setUrl(url);
                QNetworkReply *reply = m_nam->get(nr);
                QPair<RequestType, GUI::ImageLabel *>  pair(Other, l);
                m_outstandingRequests.insert(reply,pair);
                hLayout->addWidget(l,0,Qt::AlignTop);

            }
        }

    }

    // Now that our images are on their way, lets lay out the text
    //vLayout = new QVBoxLayout();
    QFont newFont;
    if (attachment->getName() != "")
    {
        // This is a horrible hack that I need to do because the URLs
        // for albums returned by FB are useless.
        QString href;
        if (savedAlbumId != "")
            href = "aid:" + savedAlbumId;
        else if (attachment->getFbObjectType() == "album")
            href = "aid:" + attachment->getFbObjectId();
        else
            href = attachment->getHref().toString();

        QLabel *name = new QLabel("<a href=\"" + href + "\">" +
                              attachment->getName() + "</a>");
        connect(name, SIGNAL(linkActivated(QString)),
                this, SIGNAL(userClickedUrl(QString)));
        name->setWordWrap(true);
        vLayout->addWidget(name,0,Qt::AlignTop);
        newFont = name->font();
    }

    // QFont newFont = font();
    newFont.setPointSize(newFont.pointSize() -2);


    if (attachment->getCaption() != "")
    {
        QLabel *caption = new QLabel(attachment->getCaption());
        connect(caption, SIGNAL(linkActivated(QString)),
                this, SIGNAL(userClickedUrl(QString)));
        caption->setWordWrap(true);
        caption->setFont(newFont);
        vLayout->addWidget(caption);
    }

    if (attachment->getDescription() != "")
    {
        QLabel *description = new QLabel(attachment->getDescription());
        connect(description, SIGNAL(linkActivated(QString)),
                this, SIGNAL(userClickedUrl(QString)));
        description->setWordWrap(true);
        description->setFont(newFont);
        vLayout->addWidget(description,1);
    }

    QList<DATA::FbStreamAttachmentProperty *> pList = attachment->getProperties();

    for (int i = 0; i < pList.size(); i++)
    {
        QString text = pList.at(i)->getName() + ": ";

        if (pList.at(i)->getHref().toString() != "")
            text.append("<a href=\"" + pList.at(i)->getHref().toString() + "\">");

        text.append(pList.at(i)->getText());
        if (pList.at(i)->getHref().toString() != "")
            text.append("</a>");

        QLabel *property = new QLabel(text);
        property->setFont(newFont);
        connect(property, SIGNAL(linkActivated(QString)),
                this, SIGNAL(userClickedUrl(QString)));
        vLayout->addWidget(property);

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
    QPair<RequestType, GUI::ImageLabel *> pair = m_outstandingRequests.take(reply);
    GUI::ImageLabel *pl = pair.second;
    RequestType type = pair.first;

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        if (p.width() > 130)
            p = p.scaledToWidth(130);
        pl->setPixmap(p);
        pl->setMinimumHeight(p.height());

        if (type == Photo)
        {
            UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();
            QString pid = pl->getUrlString().split(":").at(1);
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

void FbAttachmentWidget::userClickedVideo(QString url) {
    qDebug() << "FbAttachmentWidget::userClickedVideo(); url: " << url;
    m_videoLabel->hide();
    m_webView->setVisible(true);
    m_webView->load(QUrl(url));
    m_webView->show();
}

QSize FbAttachmentWidget::minimumSizeHint () const {
    if (sizeHint().width() > 450)
        return sizeHint();
    else return QSize(450,sizeHint().height());
}

} // namespace GUI
