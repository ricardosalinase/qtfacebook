#include "FbPhotoViewWidget.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>

#include "FbCommentManager.h"

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

    QNetworkRequest nr;
    nr.setUrl(photo->getSrcBig());
    m_nam->get(nr);


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

    }
    else
    {
        qDebug() << "Error retrieving photo: " << reply->errorString();
    }

    reply->deleteLater();

}


} // namespace GUI
