#include "FbBuddy.h"

namespace DATA {

FbBuddy::FbBuddy()
{
}

void FbBuddy::setUID(const QString &id)
{
    m_uid = id;
}

const QString& FbBuddy::getUID()
{
    return m_uid;
}

void FbBuddy::setFullName(const QString &fullName)
{
    m_fullName = fullName;
}

const QString& FbBuddy::getFullName()
{
    return m_fullName;
}

void FbBuddy::setAvatar(QPixmap avatar)
{
    m_avatar = avatar;
}

const QPixmap& FbBuddy::getAvatar()
{
    return m_avatar;
}

void FbBuddy::setWentAwayTime(const QString &time)
{
    m_wentAway = time;
}

const QString& FbBuddy::getWentAwayTime()
{
    return m_wentAway;
}

} // namespace DATA
