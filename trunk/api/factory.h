#ifndef FACTORY_H
#define FACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

#include "userinfo.h"
#include "apimethod.h"
#include "friends_get.h"
#include "comments_get.h"
#include "notifications_get.h"
#include "notifications_getlist.h"
#include "users_getloggedinuser.h"
#include "notifications_markread.h"


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
    Factory(UserInfo *userInfo, QObject *parent = 0);
    ~Factory();
    /**
    * @brief Sets the UserInfo property of the factory, replacing the existing one
    * @param userInfo
    */
    void setUserInfo(UserInfo *userInfo);

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
    /*!
    * This signal is emitted whenever a API::Comments::Get method has completed
    */
    void apiCommentsGet(API::Comments::Get *method );
    /*!
    * This signal is emitted whenever a API::Notifications::Get method has completed
    */
    void apiNotificationsGet(API::Notifications::Get *method);
    /*!
    * This signal is emitted whenever a API::Notifications::GetList method has completed
    */
    void apiNotificationsGetList(API::Notifications::GetList *method);
    /*!
    * This signal is emitted whenever a API::Notifications::MarkRead method has completed
    */
    void apiNotificationsMarkRead(API::Notifications::MarkRead *method);
    /*!
    * This signal is emitted whenever a API::Users::getLoggedInUser method has completed
    */
    void apiUsersGetLoggedInUser(API::Users::GetLoggedInUser *method);


public slots:
    void dispatch(API::Method *);

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
    UserInfo *m_userInfo;
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
