#include "FbPhotoViewWidget.h"

#include <QPixmap>
#include <QDebug>

#include "FbCommentManager.h"
#include "util/FbPhotoCache.h"
#include "util/OurUserInfo.h"
#include "util/agestring.h"

namespace GUI {

FbPhotoViewWidget::FbPhotoViewWidget(const QString& photoId, QWidget *parent) :
    QWidget(parent),
    m_photo(0),
    m_isOwner(false)
{
    // If we get a pid (photoId) rather than a photo object, we do the
    // FQL query and "own" the resulting FbPhoto object, deleting it
    // in our destructor
    m_destroyPhoto = true;

    resize(620,320);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    setPalette(palette);

    setWindowTitle("Loading Photo ...");

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_progress = new LoadingProgressWidget("Loading Photo");
    m_mainLayout->addWidget(m_progress, 2, Qt::AlignCenter);

    setLayout(m_mainLayout);
\
    m_factory = new API::Factory(this);
    connect(m_factory, SIGNAL(apiFqlGetPhotos(API::FQL::GetPhotos *)),
            this, SLOT(apiFqlGetPhotos(API::FQL::GetPhotos*)));
    connect(m_factory, SIGNAL(apiFqlGetPhotosFailed(API::FQL::GetPhotos*)),
            this, SLOT(getPhotosFailed(API::FQL::GetPhotos*)));

    API::Method *method = m_factory->createMethod("fql.multiquery.getPhotos");
    method->setArgument("pid",photoId);
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "FbPhotoViewWidget::FbPhotoViewWidget(); "
                "fql.multiquery.getPhotos failed:  " << method->errorString();
        delete method;
    }

}

FbPhotoViewWidget::FbPhotoViewWidget(DATA::FbPhoto *photo, QWidget *parent) :
    QWidget(parent),
    m_photo(photo),
    m_isOwner(false),
    m_progress(0),
    m_destroyPhoto(false)
{

    resize(620,320);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    setPalette(palette);

    setWindowTitle("Loading Photo ...");
    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    setLayout(m_mainLayout);
    buildDisplay();


}

FbPhotoViewWidget::~FbPhotoViewWidget() {

    if (m_destroyPhoto && m_photo != 0)
        delete m_photo;
}

void FbPhotoViewWidget::buildDisplay() {

    if (UTIL::OurUserInfo::getInstance()->getUID() == m_photo->getOwnerId())
        m_isOwner = true;

    if (m_photo->isUserOwned())
        this->setWindowTitle(m_photo->getUserInfo().getName());
    else
        this->setWindowTitle(m_photo->getPageInfo().getName());

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    QFrame *f = new QFrame();
    f->setMinimumWidth(10);
    f->setFrameShape(QFrame::VLine);
    m_mainLayout->insertWidget(1,f,0);
    
    m_rightSideLayout = new QVBoxLayout();
    m_rightSideLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    QString s = "<style type=\"text/css\">a { text-decoration: none; }</style>"
                "From " + this->windowTitle() + "'s album <a href=\"aid:" +
                m_photo->getAlbumId() + "\">" + m_photo->getAlbumName() +
                "</a>";

    QLabel *l = new QLabel(s);
    l->setWordWrap(true);
    connect(l, SIGNAL(linkActivated(QString)),
            this, SIGNAL(userClickedLink(QString)));
    m_rightSideLayout->addWidget(l);

    l = new QLabel("Uploaded: " + UTIL::ageString(m_photo->getCreatedTime()));
    m_rightSideLayout->addWidget(l);

    UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();

    QPixmap *p = cache->getPixmap(m_photo->getPhotoId(), UTIL::FbPhotoCache::PicBig,
                                  m_photo->getSrcBig());

    if (p == 0)
    {
        if (m_progress == 0)
        {
            m_progress = new LoadingProgressWidget("Loading Photo");
            m_mainLayout->insertWidget(0,m_progress, 1, Qt::AlignCenter);
        }
        QNetworkRequest nr;
        nr.setUrl(m_photo->getSrcBig());
        m_nam->get(nr);
    }
    else
    {
        if (m_progress != 0)
        {
            m_mainLayout->removeWidget(m_progress);
            delete m_progress;
            m_progress = 0;
        }
        QLabel *l = new QLabel();
        if (p->height() > 604)
            *p = p->scaledToHeight(604);
        else if (p->width() > 604)
            *p = p->scaledToWidth(604);
        l->setPixmap(*p);
        l->setMinimumWidth(p->width());
        l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_mainLayout->insertWidget(0,l,0,Qt::AlignTop);
        delete p;
    }

    if (m_photo->getCaption() != "")
    {
        l = new QLabel("\"" + m_photo->getCaption() + "\"");
        l->setWordWrap(true);
        m_rightSideLayout->addWidget(l);
    }

    m_rightSideLayout->addSpacing(20);

    FbCommentManager *cm = new FbCommentManager(m_photo->getObjectId(),
                                                FbCommentManager::ObjectId,
                                                m_isOwner);
    m_rightSideLayout->addWidget(cm,1);
    m_mainLayout->addLayout(m_rightSideLayout,1);

}

void FbPhotoViewWidget::apiFqlGetPhotos(API::FQL::GetPhotos *method) {

    QList<DATA::FbPhoto *> *pList = method->getPhotos();
    if (pList->size())
    {
        m_photo = pList->at(0);

        buildDisplay();

    }
    else // Facebook returned an empty result - hit bug
    {
        QLabel *l = new QLabel("Unfortunately we are unable to retrieve this "
                       "photo from Facebook.<BR><BR>There is currently a bug "
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

void FbPhotoViewWidget::getPhotosFailed(API::FQL::GetPhotos *method) {

    qDebug() << "FbPhotoViewWidget::getPhotosFailed(); " << method->getErrorStr();
    delete method;
}


void FbPhotoViewWidget::gotNetworkReply(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        UTIL::FbPhotoCache *cache = UTIL::FbPhotoCache::getInstance();
        cache->cachePixmap(m_photo->getPhotoId(), UTIL::FbPhotoCache::PicBig,
                           m_photo->getSrcBig(), p);


        QLabel *l = new QLabel();
        if (p.height() > 604)
            p = p.scaledToHeight(604);
        else if (p.width() > 604)
            p = p.scaledToWidth(604);

        l->setPixmap(p);
        l->setMinimumWidth(p.width());
        l->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        m_mainLayout->removeWidget(m_progress);
        delete m_progress;
        m_mainLayout->insertWidget(0,l,0,Qt::AlignTop);



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
