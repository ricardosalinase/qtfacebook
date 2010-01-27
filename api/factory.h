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


// Simple factory created as a singleton so that there is a single
// instance of QNetworkAcceessManager

namespace API {


/**
 * @class API::Factory factory.h api/factory.h
 * The API::Factory class is a singleton simple factory that produces API::Method derived
 * objects. These objects are the network interface to the Facebook REST API.
 */
class Factory : public QObject
{
Q_OBJECT
public:

    /**
    * @brief Returns the instance of the API::Factory.
    * @param userInfo
    * @return Factory
    */
    static Factory * getInstance(UserInfo *userInfo );

    /**
    * Returns the instance of the Factory. If the factory has not already been initialized setUserInfo() must be called
    * @return Factory
    */
    static Factory * getInstance();

    /**
    * @brief Sets the UserInfo property of the factory.
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
    * This signal is emitted whenever a API::Users::getLoggedInUser method has completed
    */
    void apiUsersGetLoggedInUser(API::Users::GetLoggedInUser *method);

public slots:
    void dispatch(API::Method *);

protected:
    Factory(QObject *parent = 0);

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
    * @brief API::Factory is a singleton. This is the private instantiation
    *
    * @var m_factory
    */
    static Factory *m_factory;
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
