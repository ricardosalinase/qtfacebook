#include "userinfo.h"

UserInfo::UserInfo(QString sessionKey, QString secret, QString uid) :
        m_sessionKey(sessionKey),
        m_secret(secret),
        m_uid(uid)
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
