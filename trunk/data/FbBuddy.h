#ifndef FBBUDDY_H
#define FBBUDDY_H

#include <QString>
#include <QPixmap>

namespace DATA {

class FbBuddy
{
public:
    explicit FbBuddy();

    void setUID(const QString& id);
    const QString& getUID();
    void setFullName(const QString& fullName);
    const QString& getFullName();
    void setAvatar(const QPixmap avatar);
    const QPixmap& getAvatar();
    void setWentAwayTime(const QString& time);
    const QString& getWentAwayTime();

private:
    QString m_uid;
    QString m_fullName;
    QPixmap m_avatar;
    QString m_wentAway;

};

} // namespace DATA

#endif // FBBUDDY_H
