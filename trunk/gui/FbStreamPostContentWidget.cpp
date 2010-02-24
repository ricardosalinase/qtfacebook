#include "FbStreamPostContentWidget.h"
#include <QLabel>
#include <QDebug>
#include <QNetworkRequest>

namespace GUI {

FbStreamPostContentWidget::FbStreamPostContentWidget(DATA::FbStreamAttachment *attachment, QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(450);
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));

    m_linkLayout = new QHBoxLayout();
    m_linkLayout->setMargin(0);
    QVBoxLayout *linkTextLayout = new QVBoxLayout();
    if (attachment->getName() != "")
    {
        QLabel *name = new QLabel("<a href=\"" + attachment->getHref().toString() + "\">" +
                              attachment->getName() + "</a>");

        name->setWordWrap(true);
        linkTextLayout->addWidget(name,0,Qt::AlignTop);
    }

    if (attachment->getCaption() != "")
    {
        QLabel *caption = new QLabel(attachment->getCaption());
        caption->setWordWrap(true);
        linkTextLayout->addWidget(caption);
    }

    if (attachment->getDescription() != "")
    {
        QLabel *description = new QLabel(attachment->getDescription());
        description->setWordWrap(true);
        linkTextLayout->addWidget(description);
    }

    QList<DATA::FbStreamAttachmentProperty *> pList = attachment->getProperties();

    for (int i = 0; i < pList.size(); i++)
    {
        linkTextLayout->addWidget(new QLabel(pList.at(i)->getName() + ": " +
                                             pList.at(i)->getText()));
    }

    m_linkLayout->insertLayout(1,linkTextLayout,1);


    QList<DATA::FbStreamMedia *> mList = attachment->getMedia();
    for (int i = 0; i < mList.size(); i++)
    {
        //if (mList.at(i)->getType() == "link")
        //{
            QNetworkRequest nr;
            nr.setUrl(mList.at(i)->getSrc());
            m_nam->get(nr);

        //}
    }
    setLayout(m_linkLayout);

}

void FbStreamPostContentWidget::gotNetworkReply(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {

        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);

        l->setMinimumHeight(p.height());
        m_linkLayout->insertWidget(0,l,0,Qt::AlignTop);
        updateGeometry();
        adjustSize();
        update();
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toEncoded();
    }

    reply->deleteLater();
}

} // namespace GUI
