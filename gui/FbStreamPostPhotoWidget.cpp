#include "FbStreamPostPhotoWidget.h"
#include <QLabel>
#include <QDebug>
#include <QNetworkRequest>

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

    for (int i = 0; i < mList.size(); i++)
    {
        QNetworkRequest nr;
        DATA::FbStreamMedia *m = mList.at(i);
        QUrl url = m->getSrc();
        nr.setUrl(url);
        m_nam->get(nr);
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

    if (reply->error() == QNetworkReply::NoError)
    {

        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);

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

} // namespace GUI
