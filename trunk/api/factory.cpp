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

Method * Factory::createMethod(QString method) {

    Q_ASSERT_X(0 != m_userInfo, "createMethod()","UserInfo object not set in factory" );

    if (method == "comments.get")
        return prepareMethod(new Comments::Get());
    else if (method == "friends.get")
        return prepareMethod(new Friends::Get());
    else if (method == "notifications.get")
        return prepareMethod(new Notifications::Get());
    else if (method == "notifications.getList")
        return prepareMethod(new Notifications::GetList());
    else if (method == "users.getLoggedInUser")
        return prepareMethod(new Users::GetLoggedInUser());
    else
        return 0;
}

Method * Factory::prepareMethod(Method *m) {

    m->setAccessManager(m_manager);
    m->setUserInfo(m_userInfo);

    connect(m,SIGNAL(methodComplete(API::Method*)),
            this, SLOT(dispatch(API::Method*)));
    return m;

}

void Factory::dispatch(API::Method *method) {

    if (method->getMethodName() == "friends.get")
        emit apiFriendsGet((API::Friends::Get*)method);
    else if (method->getMethodName() == "notifications.getList")
        emit apiNotificationsGetList((API::Notifications::GetList*)method);
    else if (method->getMethodName() == "notifications.get")
        emit apiNotificationsGet((API::Notifications::Get*)method);
    else if (method->getMethodName() == "users.getLoggedInUser")
        emit apiUsersGetLoggedInUser((API::Users::GetLoggedInUser*)method);
}

void Factory::setUserInfo(UserInfo *userInfo) {
    m_userInfo = userInfo;
}

} // namespace API
