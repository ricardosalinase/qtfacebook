#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

class UserInfo
{
public:

    UserInfo(QString sessionKey, QString secret, QString uid);

    QString getSessionKey();
    QString getSecret();
    QString getUID();

private:
    QString m_sessionKey;
    QString m_secret;
    QString m_uid;
};

#endif // USERINFO_H
