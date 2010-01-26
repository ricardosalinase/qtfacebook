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

class Factory : public QObject
{
Q_OBJECT
public:
    static Factory* getInstance();
    static Factory* getInstance(UserInfo *userInfo);

    void setUserInfo(UserInfo *userInfo);
    API::Method* createMethod(MethodType::type m);



signals:
    void apiFriendsGet(API::Friends::Get*);

public slots:
    void dispatch(API::Method *);

protected:
    Factory(QObject *parent = 0);

private:
    QNetworkAccessManager *m_manager;
    UserInfo *m_userInfo;
    static Factory *m_factory;
    Method* prepareMethod(MethodType t, Method *m);
};

} // namespace API

#endif // FACTORY_H
