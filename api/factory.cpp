#include "factory.h"

#include "util/OurUserInfo.h"


namespace API {

Factory::Factory(QObject *parent) :
    QObject(parent)
{
    m_manager = new QNetworkAccessManager();
    m_userInfo = UTIL::OurUserInfo::getInstance();
}

Factory::~Factory() {
    delete m_manager;
}

Method * Factory::createMethod(QString method) {

    Q_ASSERT_X(0 != m_userInfo, "createMethod()","UserInfo object not set in factory" );

    if (method.compare("friends.get",Qt::CaseInsensitive) == 0)
        return prepareMethod(new Friends::Get());
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
    else if (method.compare("stream.addcomment", Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::Stream::AddComment());
    else if (method.compare("fql.multiquery.getComments",Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::FQL::GetComments());
    else if (method.compare("stream.removeComment", Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::Stream::RemoveComment());
    else if (method.compare("fql.multiquery.getPhotos", Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::FQL::GetPhotos());
    else if (method.compare("comments.add", Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::Comments::Add());
    else if (method.compare("comments.remove", Qt::CaseInsensitive) == 0)
        return prepareMethod(new API::Comments::Remove());
    else if (method.compare("fql.multiquery.getAlbums", Qt::CaseInsensitive) == 0)
         return prepareMethod(new API::FQL::GetAlbums());
    else
        return 0;
}

Method * Factory::prepareMethod(Method *m) {

    m->setAccessManager(m_manager);
    //m->setUserInfo(m_userInfo);

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
    else if (method->getMethodName().compare("stream.addComment", Qt::CaseInsensitive) == 0)
        emit apiStreamAddCommentFailed((API::Stream::AddComment *)method);
    else if (method->getMethodName().compare("fql.multiquery.getComments", Qt::CaseInsensitive) == 0)
        emit apiFqlGetCommentsFailed((API::FQL::GetComments *)method);
    else if (method->getMethodName().compare("stream.removeComment", Qt::CaseInsensitive) == 0)
        emit apiStreamRemoveCommentFailed((API::Stream::RemoveComment*)method);
    else if (method->getMethodName().compare("fql.multiquery.getPhotos", Qt::CaseInsensitive) == 0)
        emit apiFqlGetPhotosFailed((API::FQL::GetPhotos *)method);
    else if (method->getMethodName().compare("comments.add", Qt::CaseInsensitive) == 0)
        emit apiCommentsAddFailed((API::Comments::Add *)method);
    else if (method->getMethodName().compare("comments.remove", Qt::CaseInsensitive) == 0)
        emit apiCommentsRemoveFailed((API::Comments::Remove *)method);
    else if (method->getMethodName().compare("fql.multiquery.getAlbums", Qt::CaseInsensitive) == 0)
        emit apiFqlGetAlbumsFailed((API::FQL::GetAlbums *)method);
}

void Factory::dispatch(API::Method *method) {

    if (method->getMethodName().compare("friends.get", Qt::CaseInsensitive) == 0)
        emit apiFriendsGet((API::Friends::Get*)method);
    else if (method->getMethodName().compare("notifications.getList", Qt::CaseInsensitive) == 0)
        emit apiNotificationsGetList((API::Notifications::GetList*)method);
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
    else if (method->getMethodName().compare("stream.addComment", Qt::CaseInsensitive) == 0)
        emit apiStreamAddComment((API::Stream::AddComment *)method);
    else if (method->getMethodName().compare("fql.multiquery.getComments", Qt::CaseInsensitive) == 0)
        emit apiFqlGetComments((API::FQL::GetComments *)method);
    else if (method->getMethodName().compare("stream.removeComment", Qt::CaseInsensitive) == 0)
            emit apiStreamRemoveComment((API::Stream::RemoveComment*)method);
    else if (method->getMethodName().compare("fql.multiquery.getPhotos", Qt::CaseInsensitive) == 0)
            emit apiFqlGetPhotos((API::FQL::GetPhotos *)method);
    else if (method->getMethodName().compare("comments.add", Qt::CaseInsensitive) == 0)
            emit apiCommentsAdd((API::Comments::Add *)method);
    else if (method->getMethodName().compare("comments.remove", Qt::CaseInsensitive) == 0)
            emit apiCommentsRemove((API::Comments::Remove *)method);
    else if (method->getMethodName().compare("fql.multiquery.getAlbums", Qt::CaseInsensitive) == 0)
            emit apiFqlGetAlbums((API::FQL::GetAlbums *)method);

}



} // namespace API
