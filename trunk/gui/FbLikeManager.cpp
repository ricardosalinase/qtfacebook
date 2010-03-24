#include "FbLikeManager.h"

#include <QHBoxLayout>
#include <QDebug>

namespace GUI {

FbLikeManager::FbLikeManager(const QString& fbObjectId, bool userLikes, QWidget *parent) :
    QWidget(parent),
    m_objectId(fbObjectId),
    m_userLikes(userLikes)
{
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(228, 232, 248));
    setPalette(palette);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(getLikes()));

    m_factory = new API::Factory(this);
    connect(m_factory, SIGNAL(apiFqlGetLikes(API::FQL::GetLikes*)),
            this, SLOT(apiFqlGetLikes(API::FQL::GetLikes*)));
    connect(m_factory, SIGNAL(apiFqlGetLikesFailed(API::FQL::GetLikes*)),
            this, SLOT(apiFqlGetLikesFailed(API::FQL::GetLikes*)));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(3);
    QPixmap p;
    p.load(":/uiImages/thumbUp_50_50.jpg");
    p = p.scaledToWidth(25);
    QLabel *icon = new QLabel();
    icon->setPixmap(p);
    layout->addWidget(icon,0);


    m_label = new QLabel();
    QFont newFont;
    newFont.setPointSize(newFont.pointSize() -2);
    m_label->setFont(newFont);
    layout->addWidget(m_label,1,Qt::AlignTop);
    layout->addStretch(2);

    setLayout(layout);
    this->setVisible(false);
    getLikes();

}

void FbLikeManager::getLikes()
{
    API::Method *method = m_factory->createMethod("fql.query.getLikes");
    method->setArgument("object_id",m_objectId);

    bool rc = method->execute();
    if (!rc)
    {
        qDebug() << "FbLikeManager::getLikes(); fwl.query.getLikes failed: " << method->getErrorStr();
        delete method;
    }


}

void FbLikeManager::apiFqlGetLikes(API::FQL::GetLikes *method)
{
    while (!m_likers.empty())
        delete m_likers.takeFirst();

    m_likers = method->getLikers();

    delete method;
    updateLikes();
    m_timer->start(60000);

}

void FbLikeManager::apiFqlGetLikesFailed(API::FQL::GetLikes *method)
{
    qDebug() << "API::FQL::GetLikes Failed. Retrying in 5 seconds";
    m_timer->start(5000);
    delete method;


}

void FbLikeManager::userLikes()
{
    m_userLikes = true;
    // do API call
    emit userChangedLike(m_userLikes);
    // update our display
    updateLikes();
}

void FbLikeManager::userStoppedLiking()
{
    m_userLikes = false;
    // do API call
    emit userChangedLike(m_userLikes);
    // update our display
    updateLikes();
}

bool FbLikeManager::toggleUserLikes()
{
    m_userLikes ^= true;
    updateLikes();
    emit userChangedLike(m_userLikes);
    return m_userLikes;
}

void FbLikeManager::updateLikes()
{
    QString likers;
    QString lastLiker;

    int numLikers = m_likers.size();

    if (numLikers)
    {
        lastLiker = m_likers.last()->getName();

        for (int i = 0; i < m_likers.size() - 1; i++)
        {
            likers.append("<b>" + m_likers.at(i)->getName() + "</b>, ");
        }

    }

    if (m_userLikes)
    {
        if (numLikers == 0)
            likers = "<b>You</b> like this.";
        else if (numLikers == 1)
            likers = "<b>You</b> and <b>" + lastLiker + "</b> like this.";
        else
        {
            likers.prepend("<b>You</b>, ");
            likers.chop(2);
            likers.append(" and <b>" + lastLiker + "</b> like this.");
        }
    }
    else
    {
        if (numLikers == 1)
            likers = "<b>" + lastLiker + "</b> likes this.";
        else
        {
            likers.chop(2);
            likers.append(" and <b>" + lastLiker + "</b> like this.");
        }
    }

    m_label->setText(likers);

    if (m_userLikes || numLikers)
        this->setVisible(true);
    else
        this->setVisible(false);


}

} // namespace GUI
