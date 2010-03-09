#include "FbAlbumViewWidget.h"

#include <QLabel>
#include <QDebug>
#include <QPushButton>

#include "util/FbPhotoCache.h"
#include "util/agestring.h"
#include "gui/FbCommentManager.h"

namespace GUI {

FbAlbumViewWidget::FbAlbumViewWidget(const QString& albumId, QWidget *parent) :
    QWidget(parent),
    m_album(0),
    m_isOwner(false),
    m_destroyAlbum(true),
    m_photoViewWidget(0)
{

    resize(620,320);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    setPalette(palette);

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
    m_destroyAlbum(false),
    m_photoViewWidget(0)
{

    resize(620,320);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    setPalette(palette);

    setWindowTitle("Loading Album ...");

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_progress = new LoadingProgressWidget("Loading Album");
    m_mainLayout->addWidget(m_progress, 2, Qt::AlignCenter);

    setLayout(m_mainLayout);

    getPhotos();

}

FbAlbumViewWidget::~FbAlbumViewWidget() {

    if (m_photoViewWidget != 0)
        delete m_photoViewWidget;


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

    if (m_album->getNumPhotos() > 15)
        m_useSmallThumbs = true;

    m_photoPaneLayout = new QVBoxLayout();

    if (m_album->getNumPhotos() > 20)
    {
        m_currentPage = 0;
        QHBoxLayout *layout = new QHBoxLayout();
        m_prev = new QPushButton("Prev");
        m_prev->setEnabled(false);
        connect(m_prev, SIGNAL(clicked()),
                this, SLOT(userClickedPrev()));
        m_next = new QPushButton("Next");
        connect(m_next, SIGNAL(clicked()),
                this, SLOT(userClickedNext()));
        m_pageReadout = new QLabel("1 - 20 of " + QString::number(m_album->getNumPhotos()));

        layout->addWidget(m_prev,0,Qt::AlignLeft);
        layout->addWidget(m_pageReadout,1,Qt::AlignCenter);
        layout->addWidget(m_next,0,Qt::AlignRight);

        m_photoPaneLayout->addLayout(layout,0);

    }



    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    m_mainLayout->removeWidget(m_progress);
    delete m_progress;
    m_progress = 0;

    m_mainLayout->insertLayout(0,m_photoPaneLayout);

    QFrame *f = new QFrame();
    f->setMinimumWidth(10);
    f->setFrameShape(QFrame::VLine);
    m_mainLayout->insertWidget(1,f,0);


    m_thumbLayout = new QGridLayout();
    m_thumbLayout->setSpacing(10);

    QWidget *page = new QWidget();
    page->setLayout(m_thumbLayout);
    m_photoPaneLayout->addWidget(page,1);
    int row = 0;
    m_pages.append(page);

    UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

    for (int i = photoList->size() - 1, rc = 0; i != -1; i--,rc++)
    {
        if ( (m_useSmallThumbs && (rc > 4)) ||
            (!m_useSmallThumbs && (rc > 3)) )
        {
            rc = 0;
            row++;
            if (row > 3)
            {
                page = new QWidget();
                m_thumbLayout = new QGridLayout();
                m_thumbLayout->setSpacing(10);
                page->setLayout(m_thumbLayout);
                m_pages.append(page);
                row = 0;
            }
        }

        DATA::FbPhoto *photo = photoList->at(i);

        GUI::FbPhotoLabel *pl = new GUI::FbPhotoLabel(photo);

        connect(pl, SIGNAL(userClickedImage(DATA::FbPhoto*)),
                this, SLOT(userClickedPhoto(DATA::FbPhoto*)));

        pl->setMinimumSize(50, 100);

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
            pl->setMinimumSize(p->size());
            delete p;
        }

        m_thumbLayout->addWidget(pl, row, rc, Qt::AlignTop | Qt::AlignCenter);

    }

    QString s;
    if (m_album->isUserOwned())
        s = m_album->getUserInfo().getName() + "'s album: ";
    else
        s = m_album->getPageInfo().getName() + "'s album: ";

    s.append(m_album->getAlbumName());
    this->setWindowTitle(s);

    QVBoxLayout *rightSideLayout = new QVBoxLayout();

    QLabel *l = new QLabel("Created: " + UTIL::ageString(m_album->getCreatedTime()));
    rightSideLayout->addWidget(l,0);

    if (m_album->getLocation() != "")
    {
        l = new QLabel("Location: " + m_album->getLocation());
        rightSideLayout->addWidget(l,0);
    }

    if (m_album->getDescription() != "")
    {
        l = new QLabel("Description: " + m_album->getDescription());
        rightSideLayout->addWidget(l,0);
    }

    rightSideLayout->addSpacing(20);

    bool isOwner = false;
    UTIL::OurUserInfo *info = UTIL::OurUserInfo::getInstance();
    if (info->getUID() == m_album->getOwnerID())
        isOwner = true;


    FbCommentManager *cm = new FbCommentManager(m_album->getObjectId(),
                                                FbCommentManager::ObjectId,
                                                isOwner);


    rightSideLayout->addWidget(cm,0);
    m_mainLayout->addLayout(rightSideLayout,2);



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
        pl->setMinimumSize(p.size());

    }
    else
    {
         qDebug() << "FbAlbumViewWidget::gotNetworkReply(); Error retrieving photo: " << reply->errorString();
    }


}

void FbAlbumViewWidget::userClickedPhoto(DATA::FbPhoto *photo) {

    qDebug() << "User clicked: " << photo->getPhotoId();


    if (m_photoViewWidget != 0)
        delete m_photoViewWidget;

    m_photoViewWidget = new GUI::FbPhotoViewWidget(photo);
    connect(m_photoViewWidget, SIGNAL(closed(GUI::FbPhotoViewWidget*)),
            this, SLOT(userClosedPhotoViewer(GUI::FbPhotoViewWidget*)));
    m_photoViewWidget->show();

    // TODO: signal the open viewer to change photos





}

void FbAlbumViewWidget::userClickedNext() {

    if (m_currentPage != m_pages.size() - 1)
    {
        m_pages.at(m_currentPage)->hide();
        m_photoPaneLayout->removeWidget(m_pages.at(m_currentPage));
        m_currentPage++;

        m_photoPaneLayout->insertWidget(1,m_pages.at(m_currentPage));
        m_pages.at(m_currentPage)->show();

        if (m_currentPage == m_pages.size() - 1)
            m_next->setEnabled(false);

        m_prev->setEnabled(true);

        QString text = QString::number((m_currentPage * 20) + 1) + " - ";

        int lastPic = (m_currentPage * 20) + 20;
        if (lastPic > m_album->getNumPhotos())
            lastPic = m_album->getNumPhotos();

        text.append(QString::number(lastPic) + " of " + QString::number(m_album->getNumPhotos()));

        m_pageReadout->setText(text);

    }

}

void FbAlbumViewWidget::userClickedPrev() {

    if (m_currentPage != 0)
    {
        m_pages.at(m_currentPage)->hide();
        m_photoPaneLayout->removeWidget(m_pages.at(m_currentPage));
        m_currentPage--;
        m_photoPaneLayout->insertWidget(1,m_pages.at(m_currentPage));
        m_pages.at(m_currentPage)->show();

        if (m_currentPage == 0)
            m_prev->setEnabled(false);

        m_next->setEnabled(true);

        QString text = QString::number((m_currentPage * 20) + 1) + " - ";

        int lastPic = (m_currentPage * 20) + 20;
        if (lastPic > m_album->getNumPhotos())
            lastPic = m_album->getNumPhotos();

        text.append(QString::number(lastPic) + " of " + QString::number(m_album->getNumPhotos()));

        m_pageReadout->setText(text);

    }

}


void FbAlbumViewWidget::userClosedPhotoViewer(GUI::FbPhotoViewWidget *) {
    //delete fv;

}

void FbAlbumViewWidget::closeEvent(QCloseEvent *event) {
    emit closed(this);
    event->accept();
}


} // namespace GUI
