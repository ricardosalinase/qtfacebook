#ifndef USERINFO_H
#define USERINFO_H
#include <QString>
#include "util/cookiejar.h"

class UserInfo
{
public:
    explicit UserInfo(QString apiKey);


    QString getSessionKey();
    void setSessionKey(QString sessionKey);
    QString getSecret();
    void setSecret(QString secretKey);
    QString getUID();
    void setUID(QString uid);
    QString getApiKey();
    void setApiKey(QString apiKey);
    QString getChannel();
    void setChannel(QString channel);
    QString getPostFormId();
    void setPostFormId(QString pfi);
    QString getEmailAddy();
    void setEmailAddy(QString email);
    QString getPass();
    void setPass(QString pass);
    UTIL::CookieJar * getCookieJar();

private:
    QString m_sessionKey;
    QString m_secret;
    QString m_uid;
    QString m_apiKey;
    QString m_channel;
    QString m_postFormId;
    QString m_email;
    QString m_pass;
    UTIL::CookieJar *m_jar;
};

#endif // USERINFO_H
