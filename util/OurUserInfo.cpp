#include "OurUserInfo.h"


namespace UTIL {

OurUserInfo * OurUserInfo::m_instance = 0;

OurUserInfo::OurUserInfo()
{
    m_jar = new UTIL::CookieJar();

}

OurUserInfo * OurUserInfo::getInstance() {
    if (!m_instance)
        m_instance = new OurUserInfo();

    return m_instance;
}

const QString& OurUserInfo::getSecret() {
    return m_secret;
}

void OurUserInfo::setSecret(const QString& secretKey) {
    m_secret = secretKey;
}

const QString& OurUserInfo::getSessionKey() {
    return m_sessionKey;
}

void OurUserInfo::setSessionKey(const QString& sessionKey) {
    m_sessionKey = sessionKey;
}

const QString& OurUserInfo::getUID() {
    return m_uid;
}

void OurUserInfo::setUID(const QString& uid) {
    m_uid = uid;
}

const QString& OurUserInfo::getApiKey() {
    return m_apiKey;
}

void OurUserInfo::setApiKey(const QString& apiKey) {
    m_apiKey = apiKey;
}


} // namespace UTIL
