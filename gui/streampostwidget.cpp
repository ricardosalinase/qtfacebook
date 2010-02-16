#include "streampostwidget.h"

#include <QGridLayout>
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

    QGridLayout *mainLayout = new QGridLayout();

    this->setWindowTitle(post->getPoster()->getName());


    QString messageHtml;
    QLabel *message = new QLabel();

    messageHtml.append(post->getMessage());
    //messageHtml.append("<BR><font style=\"font-size : 8px;\">" +
    //                   UTIL::ageString(post->getCreatedTime()) +
    //                   "</font>");
    message->setText(messageHtml);
    message->setWordWrap(true);
    mainLayout->addWidget(message,0,1,Qt::AlignTop);

    QLabel *postTimeLabel = new QLabel("<font style=\"font-size : 8px;\">" +
                                       UTIL::ageString(post->getCreatedTime()) +
                                       "</font>");
    mainLayout->addWidget(postTimeLabel,1,1,Qt::AlignBottom);


    m_commentContainer = new QWidget();
    m_commentScrollArea = new QScrollArea();

    QVBoxLayout *commentLayout = new QVBoxLayout();


    DATA::StreamCommentList *cList = post->getCommentList();

    for (int i = 0; i < cList->size(); i++)
    {
        CommentWidget *cw = new CommentWidget(cList->at(i),this);
        commentLayout->addWidget(cw);

    }

    commentLayout->addStretch();

    m_commentContainer->setLayout(commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);
    m_commentScrollArea->setWidgetResizable(true);
    m_commentScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    mainLayout->addWidget(m_commentScrollArea,2,1);
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
        ((QGridLayout *)layout())->addWidget(l,0,0,2,1);
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }




}


} // namespace GUI
