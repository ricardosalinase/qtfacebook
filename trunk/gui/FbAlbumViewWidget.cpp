#include "FbAlbumViewWidget.h"

#include <QLabel>
#include <QDebug>

#include "util/FbPhotoCache.h"


namespace GUI {

FbAlbumViewWidget::FbAlbumViewWidget(const QString& albumId, QWidget *parent) :
    QWidget(parent),
    m_album(0),
    m_isOwner(false),
    m_destroyAlbum(true)
{

    resize(620,320);
    setStyleSheet("background : white;");
    setWindowTitle("Loading Album ...");

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_progress = new LoadingProgressWidget("Loading Album");
    m_mainLayout->addWidget(m_progress, 2, Qt::AlignCenter);

    setLayout(m_mainLayout);

    m_factory = new API::Factory(this);
    connect(m_factory, SIGNAL(apiFqlGetAlbums(API::FQL::GetAlbums *)),
            this, SLOT(apiFqlGetAlbums(API::FQL::GetAlbums*)));
    connect(m_factory, SIGNAL(apiFqlGetAlbumFailed(API::FQL::GetAlbums*)),
            this, SLOT(getAlbumsFailed(API::FQL::GetAlbums*)));
    connect(m_factory, SIGNAL(apiFqlGetPhotos(API::FQL::GetPhotos *)),
            this, SLOT(apiFqlGetPhotos(API::FQL::GetPhotos*)));
    connect(m_factory, SIGNAL(apiFqlGetPhotosFailed(API::FQL::GetPhotos*)),
            this, SLOT(getPhotosFailed(API::FQL::GetPhotos*)));

    API::Method *method = m_factory->createMethod("fql.multiquery.getAlbums");
    method->setArgument("aid",albumId);
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "FbAlbumViewWidget::FbAlbumViewWidget(); "
                "fql.multiquery.getAlbums failed:  " << method->getErrorStr();
        delete method;
    }


}

FbAlbumViewWidget::FbAlbumViewWidget(DATA::FbAlbum *album, QWidget *parent) :
    QWidget(parent),
    m_album(album),
    m_isOwner(false),
    m_destroyAlbum(false)
{

    resize(620,320);
    setStyleSheet("background : white;");
    setWindowTitle("Loading Album ...");

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_progress = new LoadingProgressWidget("Loading Album");
    m_mainLayout->addWidget(m_progress, 2, Qt::AlignCenter);

    setLayout(m_mainLayout);

    getPhotos();

}

FbAlbumViewWidget::~FbAlbumViewWidget() {
    if (m_destroyAlbum && m_album != 0)
        delete m_album;
}

void FbAlbumViewWidget::apiFqlGetAlbums(API::FQL::GetAlbums *method) {

    QList<DATA::FbAlbum *> *aList = method->getAlbums();
    if (aList->size())
    {
        m_album = aList->at(0);

        getPhotos();

    }
    else // Facebook returned an empty result - hit bug
    {
        QLabel *l = new QLabel("Unfortunately we are unable to retrieve this "
                       "album from Facebook.<BR><BR>There is currently a bug "
                       "in their API that causes this to occur "
                       "and we have opened "
                       "a ticket in their bugzilla system.");
        m_mainLayout->removeWidget(m_progress);
        delete m_progress;
        m_progress = 0;
        l->setWordWrap(true);
        l->setAlignment(Qt::AlignCenter);
        l->setMinimumSize(400,100);
        m_mainLayout->insertWidget(0,l,0,Qt::AlignCenter);

    }

    delete aList;
    delete method;

}


void FbAlbumViewWidget::getAlbumsFailed(API::FQL::GetAlbums *method) {

    qDebug() << "FbAlbumViewWidget::getAlbumsFailed(); " << method->getErrorStr();
    delete method;
}


void FbAlbumViewWidget::getPhotos() {

    API::Method *method = m_factory->createMethod("fql.multiquery.getPhotos");
    method->setArgument("aid", m_album->getAlbumId());
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "FbAlbumViewWidget::getPhotos(); "
                "fql.multiquery.getPhotos failed:  " << method->getErrorStr();
        delete method;
    }


}

void FbAlbumViewWidget::apiFqlGetPhotos(API::FQL::GetPhotos *method) {

    QList<DATA::FbPhoto *> *pList = method->getPhotos();
    if (pList->size())
    {
        buildDisplay(pList);

    }
    else // Facebook returned an empty result - hit bug
    {
        QLabel *l = new QLabel("Unfortunately we are unable to retrieve these "
                       "photos from Facebook.<BR><BR>There is currently a bug "
                       "in their API that causes this to occur "
                       "and we have opened "
                       "a ticket in their bugzilla system.");
        m_mainLayout->removeWidget(m_progress);
        delete m_progress;
        m_progress = 0;
        l->setWordWrap(true);
        l->setAlignment(Qt::AlignCenter);
        l->setMinimumSize(400,100);
        m_mainLayout->insertWidget(0,l,0,Qt::AlignCenter);

    }

    delete pList;
    delete method;


}

void FbAlbumViewWidget::getPhotosFailed(API::FQL::GetPhotos *method) {

    qDebug() << "FbAlbumViewWidget::getPhotosFailed(); " << method->getErrorStr();
    delete method;
}

void FbAlbumViewWidget::buildDisplay(QList<DATA::FbPhoto *> *photoList) {

    m_useSmallThumbs = false;

    if (m_album->getNumPhotos() > 12)
        m_useSmallThumbs = true;

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    m_mainLayout->removeWidget(m_progress);
    delete m_progress;
    m_progress = 0;

    m_thumbLayout = new QGridLayout();
    m_thumbLayout->setSpacing(10);
    m_mainLayout->insertLayout(0, m_thumbLayout);
    int row = 0;

    UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

    for (int i = 0, rc = 0; i < photoList->size(); i++,rc++)
    {
        if (rc > 5)
        {
            row++;
            rc = 0;
        }

        DATA::FbPhoto *photo = photoList->at(i);

        GUI::FbPhotoLabel *pl = new GUI::FbPhotoLabel(photo);
        connect(pl, SIGNAL(userClickedImage(DATA::FbPhoto*)),
                this, SLOT(userClickedPhoto(DATA::FbPhoto*)));

        QPixmap *p;

        if (m_useSmallThumbs)
            p = cache->getPixmap(photo->getPhotoId(), UTIL::FbPhotoCache::PicSmall,
                                      photo->getSrcSmall());
        else
            p = cache->getPixmap(photo->getPhotoId(), UTIL::FbPhotoCache::Pic,
                                      photo->getSrc());

        if (p == 0)
        {
            QNetworkRequest nr;

            if (m_useSmallThumbs)
                nr.setUrl(photo->getSrcSmall());
            else
                nr.setUrl(photo->getSrc());

            QNetworkReply *reply = m_nam->get(nr);
            m_outstandingRequests.insert(reply,pl);

        }
        else
        {
            pl->setPixmap(*p);
            delete p;
        }

        m_thumbLayout->addWidget(pl, row, rc, Qt::AlignTop | Qt::AlignCenter);

    }


}

void FbAlbumViewWidget::gotNetworkReply(QNetworkReply *reply) {

    GUI::FbPhotoLabel *pl = m_outstandingRequests.take(reply);

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

        if (m_useSmallThumbs)
            cache->cachePixmap(pl->getPhoto()->getPhotoId(), UTIL::FbPhotoCache::PicSmall,
                               pl->getPhoto()->getSrcSmall(), p);
        else
            cache->cachePixmap(pl->getPhoto()->getPhotoId(), UTIL::FbPhotoCache::Pic,
                               pl->getPhoto()->getSrc(), p);

        pl->setPixmap(p);

    }
    else
    {
         qDebug() << "FbAlbumViewWidget::gotNetworkReply(); Error retrieving photo: " << reply->errorString();
    }


}

void FbAlbumViewWidget::userClickedPhoto(DATA::FbPhoto *photo) {

    qDebug() << "User clicked: " << photo->getPhotoId();

}


void FbAlbumViewWidget::closeEvent(QCloseEvent *event) {
    emit closed(this);
    event->accept();
}


} // namespace GUI
