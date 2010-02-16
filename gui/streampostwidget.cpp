#include "streampostwidget.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDateTime>
#include <QDebug>

#include "util/agestring.h"
#include "commentwidget.h"


namespace GUI {

StreamPostWidget::StreamPostWidget(DATA::StreamPost *post, QWidget *parent) :
    QWidget(parent),
    m_post(post)
{

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *contentLayout = new QVBoxLayout();
    this->setWindowTitle(post->getPoster()->getName());


    QString messageHtml;
    QLabel *message = new QLabel();

    messageHtml.append(post->getMessage());
    messageHtml.append("<BR><BR><font style=\"font-size : 8px;\">" +
                       UTIL::ageString(post->getCreatedTime()) +
                       "</font>");
    message->setText(messageHtml);
    message->setWordWrap(true);
    contentLayout->addWidget(message,0,Qt::AlignTop);


    m_commentContainer = new QWidget();
    m_commentScrollArea = new QScrollArea();

    QVBoxLayout *commentLayout = new QVBoxLayout();


    DATA::StreamCommentList *cList = post->getCommentList();

    for (int i = 0; i < cList->size(); i++)
    {
        CommentWidget *cw = new CommentWidget(cList->at(i),this);
        commentLayout->insertWidget(0,cw);

    }

    commentLayout->addStretch();

    m_commentContainer->setLayout(commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);
    m_commentScrollArea->setWidgetResizable(true);
    m_commentScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    contentLayout->addWidget(m_commentScrollArea,1);
    mainLayout->insertLayout(1,contentLayout,1);
    setLayout(mainLayout);

    getPosterPixmap();


}


void StreamPostWidget::getPosterPixmap() {

    m_nam = new QNetworkAccessManager(this);

    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotPosterPixmap(QNetworkReply*)));

    QNetworkRequest nr;
    nr.setUrl(m_post->getPoster()->getPicBig());
    m_nam->get(nr);

}

void StreamPostWidget::gotPosterPixmap(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap p;
        p.loadFromData(reply->readAll());
        QLabel *l = new QLabel();
        l->setPixmap(p);
        ((QHBoxLayout *)layout())->insertWidget(0,l,0,Qt::AlignTop);
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }




}


} // namespace GUI
