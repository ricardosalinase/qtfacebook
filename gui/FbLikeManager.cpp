#include "FbLikeManager.h"

#include <QHBoxLayout>

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
    updateLikes();

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
    int numLikers = 0;

    if (m_userLikes)
        likers.append("You,");






    if (likers != "")
    {
        likers.chop(1);
        likers.append(" like");
        if (numLikers > 0)
            likers.append("s");
        likers.append(" this.");
        m_label->setText(likers);
        this->setVisible(true);
    }
    else
    {
        m_label->setText(likers);
        this->setVisible(false);
    }

}

} // namespace GUI
