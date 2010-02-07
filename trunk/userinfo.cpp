#include "userinfo.h"

UserInfo::UserInfo(QString apiKey) :
        m_apiKey(apiKey)
{
    m_jar = new UTIL::CookieJar();
}



QString UserInfo::getSecret() {
    return m_secret;
}

void UserInfo::setSecret(QString secretKey) {
    m_secret = secretKey;
}

QString UserInfo::getSessionKey() {
    return m_sessionKey;
}

void UserInfo::setSessionKey(QString sessionKey) {
    m_sessionKey = sessionKey;
}

QString UserInfo::getUID() {
    return m_uid;
}

void UserInfo::setUID(QString uid) {
    m_uid = uid;
}

QString UserInfo::getApiKey() {
    return m_apiKey;
}

void UserInfo::setApiKey(QString apiKey) {
    m_apiKey = apiKey;
}

QString UserInfo::getChannel() {
    return m_channel;
}

void UserInfo::setChannel(QString channel) {
    m_channel = channel;
}

QString UserInfo::getPostFormId() {
    return m_postFormId;
}

void UserInfo::setPostFormId(QString pfi) {
    m_postFormId = pfi;
}

QString UserInfo::getEmailAddy() {
    return m_email;
}

void UserInfo::setEmailAddy(QString email) {
    m_email = email;
}

QString UserInfo::getPass() {
    return m_pass;
}

void UserInfo::setPass(QString pass) {
    m_pass = pass;
}

UTIL::CookieJar * UserInfo::getCookieJar() {
    return m_jar;
}
