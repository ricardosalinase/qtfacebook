#include "userinfo.h"

UserInfo::UserInfo(QString sessionKey, QString secret, QString uid, QString apiKey) :
        m_sessionKey(sessionKey),
        m_secret(secret),
        m_uid(uid),
        m_apiKey(apiKey)
{
}

QString UserInfo::getSecret() {
    return m_secret;
}

QString UserInfo::getSessionKey() {
    return m_sessionKey;
}

QString UserInfo::getUID() {
    return m_uid;
}

QString UserInfo::getApiKey() {
    return m_apiKey;
}
