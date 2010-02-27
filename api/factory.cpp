#include "factory.h"




namespace API {



Factory::Factory(UserInfo *userInfo, QObject *parent) :
    QObject(parent)
{
    m_manager = new QNetworkAccessManager();
    m_userInfo = userInfo;
}

Factory::~Factory() {
    delete m_manager;
}

Method * Factory::createMethod(QString method) {

    Q_ASSERT_X(0 != m_userInfo, "createMethod()","UserInfo object not set in factory" );

    if (method.compare("comments.get",Qt::CaseInsensitive) == 0)
        return prepareMethod(new Comments::Get());
    else if (method.compare("friends.get",Qt::CaseInsensitive) == 0)
        return prepareMethod(new Friends::Get());
    else if (method.compare("notifications.get", Qt::CaseInsensitive) == 0)
        return prepareMethod(new Notifications::Get());
    else if (method.compare("notifications.getList", Qt::CaseInsensitive) == 0)
        return prepareMethod(new Notifications::GetList());
    else if (method.compare("users.getLoggedInUser", Qt::CaseInsensitive) == 0)
        return prepareMethod(new Users::GetLoggedInUser());
    else if (method.compare("notifications.markRead", Qt::CaseInsensitive) == 0)
        return prepareMethod(new Notifications::MarkRead());
    else if (method.compare("fql.multiquery.getNewNotifications", Qt::CaseInsensitive) == 0)
        return prepareMethod(new FQL::GetNewNotifications());
    else if (method.compare("fql.query.getAppInfo", Qt::CaseInsensitive) == 0)
        return prepareMethod(new FQL::GetAppInfo());
    else if (method.compare("fql.multiquery.getStreamPosts",Qt::CaseInsensitive) == 0)
        return prepareMethod(new FQL::GetStreamPosts());
    else if (method.compare("fql.multiquery.getStreamPostInfo", Qt::CaseInsensitive) == 0)
        return prepareMethod(new FQL::GetStreamPostInfo());
    else if (method.compare("stream.addcomment", Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::Stream::AddComment());
    else if (method.compare("fql.multiquery.getComments",Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::FQL::GetComments());
    else
        return 0;
}

Method * Factory::prepareMethod(Method *m) {

    m->setAccessManager(m_manager);
    m->setUserInfo(m_userInfo);

    connect(m,SIGNAL(methodComplete(API::Method*)),
            this, SLOT(dispatch(API::Method*)));
    connect(m,SIGNAL(methodFailed(API::Method*)),
            this,SLOT(dispatchFailed(API::Method*)));

    return m;

}

void Factory::dispatchFailed(API::Method *method) {
    if (method->getMethodName().compare("friends.get", Qt::CaseInsensitive) == 0)
        emit apiFriendsGetFailed((API::Friends::Get*)method);
    else if (method->getMethodName().compare("notifications.getList", Qt::CaseInsensitive) == 0)
        emit apiNotificationsGetListFailed((API::Notifications::GetList*)method);
    else if (method->getMethodName().compare("notifications.get", Qt::CaseInsensitive) == 0)
        emit apiNotificationsGetFailed((API::Notifications::Get*)method);
    else if (method->getMethodName().compare("users.getLoggedInUser", Qt::CaseInsensitive) == 0)
        emit apiUsersGetLoggedInUserFailed((API::Users::GetLoggedInUser*)method);
    else if (method->getMethodName().compare("notifications.markRead", Qt::CaseInsensitive) == 0)
        emit apiNotificationsMarkReadFailed((API::Notifications::MarkRead*)method);
    else if (method->getMethodName().compare("fql.multiquery.getNewNotifications", Qt::CaseInsensitive) == 0)
        emit apiFqlGetNewNotificationsFailed((API::FQL::GetNewNotifications*)method);
    else if (method->getMethodName().compare("fql.query.getAppInfo", Qt::CaseInsensitive) == 0 )
        emit apiFqlGetAppInfoFailed((API::FQL::GetAppInfo*)method);
    else if (method->getMethodName().compare("fql.multiquery.getStreamposts", Qt::CaseInsensitive) == 0)
        emit apiFqlGetStreamPostsFailed((API::FQL::GetStreamPosts*)method);
    else if (method->getMethodName().compare("fql.multiquery.getStreamPostInfo", Qt::CaseInsensitive) == 0)
        emit apiFqlGetStreamPostInfoFailed((API::FQL::GetStreamPostInfo*)method);
    else if (method->getMethodName().compare("stream.addComment", Qt::CaseInsensitive) == 0)
        emit apiStreamAddCommentFailed((API::Stream::AddComment *)method);
    else if (method->getMethodName().compare("fql.multiquery.getComments", Qt::CaseInsensitive) == 0)
        emit apiFqlGetCommentsFailed((API::FQL::GetComments *)method);
}

void Factory::dispatch(API::Method *method) {

    if (method->getMethodName().compare("friends.get", Qt::CaseInsensitive) == 0)
        emit apiFriendsGet((API::Friends::Get*)method);
    else if (method->getMethodName().compare("notifications.getList", Qt::CaseInsensitive) == 0)
        emit apiNotificationsGetList((API::Notifications::GetList*)method);
    else if (method->getMethodName().compare("notifications.get", Qt::CaseInsensitive) == 0)
        emit apiNotificationsGet((API::Notifications::Get*)method);
    else if (method->getMethodName().compare("users.getLoggedInUser", Qt::CaseInsensitive) == 0)
        emit apiUsersGetLoggedInUser((API::Users::GetLoggedInUser*)method);
    else if (method->getMethodName().compare("notifications.markRead", Qt::CaseInsensitive) == 0)
        emit apiNotificationsMarkRead((API::Notifications::MarkRead*)method);
    else if (method->getMethodName().compare("fql.multiquery.getNewNotifications", Qt::CaseInsensitive) == 0)
        emit apiFqlGetNewNotifications((API::FQL::GetNewNotifications*)method);
    else if (method->getMethodName().compare("fql.query.getAppInfo", Qt::CaseInsensitive) == 0 )
        emit apiFqlGetAppInfo((API::FQL::GetAppInfo*)method);
    else if (method->getMethodName().compare("fql.multiquery.getStreamposts", Qt::CaseInsensitive) == 0)
        emit apiFqlGetStreamPosts((API::FQL::GetStreamPosts*)method);
    else if (method->getMethodName().compare("fql.multiquery.getStreamPostInfo", Qt::CaseInsensitive) == 0)
        emit apiFqlGetStreamPostInfo((API::FQL::GetStreamPostInfo*)method);
    else if (method->getMethodName().compare("stream.addComment", Qt::CaseInsensitive) == 0)
        emit apiStreamAddComment((API::Stream::AddComment *)method);
    else if (method->getMethodName().compare("fql.multiquery.getComments", Qt::CaseInsensitive) == 0)
        emit apiFqlGetComments((API::FQL::GetComments *)method);
}

void Factory::setUserInfo(UserInfo *userInfo) {
    m_userInfo = userInfo;
}

} // namespace API
