#include "factory.h"




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

Method * Factory::createMethod(MethodType::type t) {

    Q_ASSERT_X(0 != m_userInfo, "createMethod()","UserInfo object not set in factory" );

    switch (t) {
    case MethodType::COMMENTS_GET:
        return prepareMethod(MethodType(t), new Comments::Get());
        break;
    case MethodType::FRIENDS_GET:
        return prepareMethod(MethodType(t), new Friends::Get());
        break;
    case MethodType::NOTIFICATIONS_GET:
        return prepareMethod(MethodType(t), new Notifications::Get());
        break;
    case MethodType::NOTIFICATIONS_GETLIST:
        return prepareMethod(MethodType(t), new Notifications::GetList());
        break;
    case MethodType::USERS_GETLOGGEDINUSER:
        return prepareMethod(MethodType(t), new Users::GetLoggedInUser());
        break;
    default:
        return 0;
        break;
    }

}

Method * Factory::prepareMethod(MethodType t, Method *m) {

    m->setAccessManager(m_manager);
    m->setUserInfo(m_userInfo);
    m->setMethodType(t);
    connect(m,SIGNAL(methodComplete(API::Method*)),
            this, SLOT(dispatch(API::Method*)));
    return m;

}

void Factory::dispatch(API::Method *method) {

    if (method->getMethodType() == API::MethodType::FRIENDS_GET)
        emit apiFriendsGet((API::Friends::Get*)method);
}

void Factory::setUserInfo(UserInfo *userInfo) {
    m_userInfo = userInfo;
}

} // namespace API
