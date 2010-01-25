#include "factory.h"

#include "friends_get.h"
#include "comments_get.h"

namespace API {

Factory* Factory::m_factory = 0;

Factory::Factory(QObject *parent) :
    QObject(parent)
{
    m_manager = new QNetworkAccessManager();
}

Factory * Factory::getInstance() {

    if (m_factory == 0) {
        m_factory = new Factory();
    }

    return m_factory;

}

Factory * Factory::getInstance(UserInfo *userInfo) {
    if (m_factory == 0) {
        m_factory = new Factory();
        m_factory->setUserInfo(userInfo);
    }
    return m_factory;
}

Method * Factory::createMethod(apiMethod m) {

    Q_ASSERT_X(0 != m_userInfo, "createMethod()","UserInfo object not set in factory" );

    switch (m) {
    case API_COMMENTS_GET:
        return prepareMethod(new Comments::Get());
        break;
    case API_FRIENDS_GET:
        return prepareMethod(new Friends::Get());
        break;
    default:
        return 0;
        break;
    }




}

Method * Factory::prepareMethod(Method *m) {

    m->setAccessManager(m_manager);
    m->setUserInfo(m_userInfo);

    return m;

}

void Factory::setUserInfo(UserInfo *userInfo) {
    m_userInfo = userInfo;
}

} // namespace API
