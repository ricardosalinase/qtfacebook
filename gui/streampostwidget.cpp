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

    QVBoxLayout *mainContentLayout = new QVBoxLayout();

    QString messageHtml;
    QLabel *message = new QLabel();

    messageHtml.append(post->getMessage());

    message->setText(messageHtml);
    message->setWordWrap(true);
    mainContentLayout->addWidget(message,0,Qt::AlignTop | Qt::AlignLeft);


    QString postTimeHtml = "<font style=\"font-size : 8px;\">" +
                           UTIL::ageString(post->getCreatedTime()) +
                           "</font>";

    QLabel *postTimeLabel = new QLabel(postTimeHtml);
    mainContentLayout->addWidget(postTimeLabel,0,Qt::AlignBottom);
    m_commentContainer = new QWidget();
    m_commentScrollArea = new QScrollArea();

    QVBoxLayout *commentLayout = new QVBoxLayout();


    DATA::StreamCommentList *cList = post->getCommentList();

    for (int i = 0; i < cList->size(); i++)
    {
        CommentWidget *cw = new CommentWidget(cList->at(i),this);
        commentLayout->addWidget(cw);

    }

    m_commentContainer->setLayout(commentLayout);

    m_commentScrollArea->setWidget(m_commentContainer);
    m_commentScrollArea->setWidgetResizable(true);

    mainContentLayout->addWidget(m_commentScrollArea,0,Qt::AlignTop);

    mainLayout->addLayout(mainContentLayout,0,1,Qt::AlignTop | Qt::AlignLeft);
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
        ((QGridLayout *)layout())->addWidget(l,0,0,Qt::AlignTop);
    }
    else
    {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }




}


} // namespace GUI
