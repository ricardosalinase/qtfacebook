#ifndef FACTORY_H
#define FACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

#include "userinfo.h"
#include "apimethod.h"

// Simple factory created as a singleton so that there is a single
// instance of QNetworkAcceessManager



namespace API {

class Factory : public QObject
{
Q_OBJECT
public:
    static Factory* getInstance();
    static Factory* getInstance(UserInfo *userInfo);

    enum apiMethod { API_FRIENDS_GET, API_COMMENTS_GET, API_NOTIFICATIONS_GET, API_NOTIFICATIONS_GETLIST };

    void setUserInfo(UserInfo *userInfo);
    API::Method* createMethod(apiMethod m);



signals:

public slots:

protected:
    Factory(QObject *parent = 0);

private:
    QNetworkAccessManager *m_manager;
    UserInfo *m_userInfo;
    static Factory *m_factory;
    Method* prepareMethod(Method *m);
};

} // namespace API

#endif // FACTORY_H
