#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

class UserInfo
{
public:

    UserInfo(QString sessionKey, QString secret, QString uid, QString apiKey);

    QString getSessionKey();
    QString getSecret();
    QString getUID();
    QString getApiKey();

private:
    QString m_sessionKey;
    QString m_secret;
    QString m_uid;
    QString m_apiKey;
};

#endif // USERINFO_H
