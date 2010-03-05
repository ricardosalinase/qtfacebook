#ifndef OURUSERINFO_H
#define OURUSERINFO_H

#include <QString>
#include "util/cookiejar.h"

namespace UTIL {

class OurUserInfo
{
public:
    static OurUserInfo * getInstance();

    const QString& getSessionKey();
    void setSessionKey(const QString& sessionKey);
    const QString& getSecret();
    void setSecret(const QString& secretKey);
    const QString& getUID();
    void setUID(const QString& uid);
    const QString& getApiKey();
    void setApiKey(const QString& apiKey);
    UTIL::CookieJar * getCookieJar();

private:
    OurUserInfo();
    ~OurUserInfo() {}

    static OurUserInfo *m_instance;
    QString m_sessionKey;
    QString m_secret;
    QString m_uid;
    QString m_apiKey;

    UTIL::CookieJar *m_jar;

};

} // namespace UTIL

#endif // OURUSERINFO_H
