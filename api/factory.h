#ifndef FACTORY_H
#define FACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

#include "util/OurUserInfo.h"
#include "apimethod.h"
#include "friends_get.h"
#include "notifications_getlist.h"
#include "users_GetLoggedInUser.h"
#include "notifications_MarkRead.h"
#include "fql_GetNewNotifications.h"
#include "fql_GetAppInfo.h"
#include "fql_GetStreamPosts.h"
#include "stream_AddComment.h"
#include "fql_GetComments.h"
#include "stream_RemoveComment.h"
#include "fql_GetPhotos.h"
#include "comments_Add.h"
#include "comments_Remove.h"
#include "api/fql_GetAlbums.h"
#include "api/fql_GetLikes.h"
#include "api/stream_AddLike.h"
#include "api/stream_RemoveLike.h"


// Simple factory

namespace API {


/**
 * @class API::Factory factory.h api/factory.h
 * The API::Factory class is a simple factory that produces API::Method derived
 * objects. These objects are the network interface to the Facebook REST API.
 */
class Factory : public QObject
{
Q_OBJECT
public:

    /*!
     * Constructs a factory which will use the data in the supplied UserInfo object for making API calls to facebook
     */
    explicit Factory(QObject *parent = 0);
    ~Factory();

    /**
    * @brief Returns a pointer to the requested API call, cast as the base API::Method class.
    * @param method
    * @return API::Method*
    */
    API::Method* createMethod(QString method);


signals:
    /*!
    * This signal is emitted whenever a API::Friends::Get method has completed
    */
    void apiFriendsGet(API::Friends::Get *method);
    void apiFriendsGetFailed(API::Friends::Get *method);
    /*!
    * This signal is emitted whenever a API::Notifications::GetList method has completed
    */
    void apiNotificationsGetList(API::Notifications::GetList *method);
    void apiNotificationsGetListFailed(API::Notifications::GetList *method);
    /*!
    * This signal is emitted whenever a API::Notifications::MarkRead method has completed
    */
    void apiNotificationsMarkRead(API::Notifications::MarkRead *method);
    void apiNotificationsMarkReadFailed(API::Notifications::MarkRead *method);
    /*!
    * This signal is emitted whenever a API::Users::getLoggedInUser method has completed
    */
    void apiUsersGetLoggedInUser(API::Users::GetLoggedInUser *method);
    void apiUsersGetLoggedInUserFailed(API::Users::GetLoggedInUser *method);
    /*!
    * This signal is emitted whenever a API::FQL::GetNewNotifications method has completed
    */
    void apiFqlGetNewNotifications(API::FQL::GetNewNotifications *method);
    void apiFqlGetNewNotificationsFailed(API::FQL::GetNewNotifications *method);
    /*!
    * This signal is emitted whenever a API::FQL::GetAppInfo method has completed
    */
    void apiFqlGetAppInfo(API::FQL::GetAppInfo *method);
    void apiFqlGetAppInfoFailed(API::FQL::GetAppInfo *method);
    /*!
     * This signal is emitted whenever a API::FQL::GetSteamPosts method has completed
     */
    void apiFqlGetStreamPosts(API::FQL::GetStreamPosts *method);
    void apiFqlGetStreamPostsFailed(API::FQL::GetStreamPosts *method);
    /*!
     * This signal is emitted whenever a API::Stream::AddComment method has completed
     */
    void apiStreamAddComment(API::Stream::AddComment *method);
    void apiStreamAddCommentFailed(API::Stream::AddComment *method);
    /*!
     * This signal is emitted whenever a API::FQL::GetComments method has completed
     */
    void apiFqlGetComments(API::FQL::GetComments *method);
    void apiFqlGetCommentsFailed(API::FQL::GetComments *method);
    /*!
     * This signal is emitted whenever a API::Stream::RemoveComment method has completed
     */
    void apiStreamRemoveComment(API::Stream::RemoveComment *method);
    void apiStreamRemoveCommentFailed(API::Stream::RemoveComment *method);
    /*!
     * This signal is emitted whenever a API::FQL::GetPhotos method has completed
     */
    void apiFqlGetPhotos(API::FQL::GetPhotos *method);
    void apiFqlGetPhotosFailed(API::FQL::GetPhotos *method);
    /*!
     * This signal is emitted whenever a API::Comments::Add method has completed
     */
    void apiCommentsAdd(API::Comments::Add *method);
    void apiCommentsAddFailed(API::Comments::Add *method);
    /*!
     * This signal is emitted whenever a API::Comments::Remove method has completed
     */
    void apiCommentsRemove(API::Comments::Remove *method);
    void apiCommentsRemoveFailed(API::Comments::Remove *method);
    /*!
     * This signal is emitted whenever a API::FQL::GetAlbums method has completed
     */
    void apiFqlGetAlbums(API::FQL::GetAlbums *method);
    void apiFqlGetAlbumsFailed(API::FQL::GetAlbums *method);
    /*!
     * This signal is emitted whenever a API::FQL::GetLikes method has completed
     */
    void apiFqlGetLikes(API::FQL::GetLikes *method);
    void apiFqlGetLikesFailed(API::FQL::GetLikes *method);
    /*!
     * This signal is emitted whenever a API::Stream::AddLike method has completed
     */
    void apiStreamAddLike(API::Stream::AddLike *method);
    void apiStreamAddLikeFailed(API::Stream::AddLike *method);
    /*!
     * This signal is emitted whenever a API::Stream::RemoveLike method has completed
     */
    void apiStreamRemoveLike(API::Stream::RemoveLike *method);
    void apiStreamRemoveLikeFailed(API::Stream::RemoveLike *method);

public slots:
    void dispatch(API::Method *);
    void dispatchFailed(API::Method *);

private:
    /**
    * @brief The private QNetworkAccessManager
    *
    * @var m_manager
    */
    QNetworkAccessManager *m_manager;
    /**
    * @brief The private UserInfo
    *
    * @var m_userInfo
    */
    UTIL::OurUserInfo *m_userInfo;
    /**
    * @brief Private method that sets the UserInfo and QNetworkManager or the produced API::Method
    *
    * @fn prepareMethod
    * @param m
    * @return Method
    */
    Method* prepareMethod(Method *m);
};

} // namespace API

#endif // FACTORY_H
