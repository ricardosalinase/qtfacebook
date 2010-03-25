#include "FbLikeManager.h"

#include <QHBoxLayout>
#include <QDebug>

namespace GUI {

FbLikeManager::FbLikeManager(const QString& fbObjectId, bool userLikes, QWidget *parent) :
    QWidget(parent),
    m_userLikes(userLikes)
{
    // Deal with their screwed up API inconsistency
    if (fbObjectId.contains("_"))
    {
        m_objectId = fbObjectId.split("_").at(1);
        m_postId = fbObjectId;
    }
    else
        m_objectId = fbObjectId;


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
    connect(m_factory, SIGNAL(apiStreamAddLike(API::Stream::AddLike*)),
            this, SLOT(apiStreamAddLike(API::Stream::AddLike*)));
    connect(m_factory, SIGNAL(apiStreamAddLikeFailed(API::Stream::AddLike*)),
            this, SLOT(apiStreamAddLikeFailed(API::Stream::AddLike*)));
    connect(m_factory, SIGNAL(apiStreamRemoveLike(API::Stream::RemoveLike*)),
            this, SLOT(apiStreamRemoveLike(API::Stream::RemoveLike*)));
    connect(m_factory, SIGNAL(apiStreamRemoveLikeFailed(API::Stream::RemoveLike*)),
            this, SLOT(apiStreamRemoveLikeFailed(API::Stream::RemoveLike*)));


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
    API::Method *method = m_factory->createMethod("stream.addLike");
    method->setArgument("post_id",m_postId);
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "FbLikeManager::userLikes(); stream.addLike failed: " << method->getErrorStr();
        delete method;
    }

    emit userChangedLike(m_userLikes);
    // update our display
    updateLikes();
}

void FbLikeManager::apiStreamAddLike(API::Stream::AddLike *method)
{
    delete method;

}

void FbLikeManager::apiStreamAddLikeFailed(API::Stream::AddLike *method)
{
    qDebug() << "FbLikeManager::apiStreamAddLikeFailed(); retrying in 5 seconds";
    QTimer::singleShot(5000, this, SLOT(userLikes()));
    delete method;
}

void FbLikeManager::userStoppedLiking()
{
    m_userLikes = false;
    // do API call
    API::Method *method = m_factory->createMethod("stream.removeLike");
    method->setArgument("post_id",m_postId);
    bool rc = method->execute();

    if (!rc)
    {
        qDebug() << "FbLikeManager::userStoppedLiking(); stream.removeLike failed: " << method->getErrorStr();
        delete method;
    }


    emit userChangedLike(m_userLikes);
    // update our display
    updateLikes();
}

void FbLikeManager::apiStreamRemoveLike(API::Stream::RemoveLike *method)
{
    delete method;

}

void FbLikeManager::apiStreamRemoveLikeFailed(API::Stream::RemoveLike *method)
{
    delete method;
    qDebug() << "FbLikeManager::apiStreamRemoveLikeFailed(); retrying in 5 seconds";
    QTimer::singleShot(5000, this, SLOT(userStoppedLiking()));
}



bool FbLikeManager::toggleUserLikes()
{
    m_userLikes ^= true;

    if (m_userLikes)
        userLikes();
    else
        userStoppedLiking();

    return m_userLikes;
}

void FbLikeManager::updateLikes()
{
    QString likers;
    QString lastLiker;

    int numLikers = m_likers.size();


    // There's some hacking around with this logic to deal with the fact that
    // you may or may not appear in the list of likers. Their API is
    // horribly broken regarding likes.

    QString myUID = UTIL::OurUserInfo::getInstance()->getUID();


    if (numLikers)
    {
        for (int i =  m_likers.size() - 1; i != -1; i--)
        {
            if (m_likers.at(i)->getUID() == myUID)
            {
                continue;
            }
            else
            {
                lastLiker = m_likers.at(i)->getName();
                break;
            }
        }

        for (int i = 0; i < m_likers.size() - 1; i++)
        {
            if (m_likers.at(i)->getUID() == myUID)
            {
                m_userLikes = true;
                numLikers--;
            }
            else if (m_likers.at(i)->getName() == lastLiker)
                continue;
            else
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
