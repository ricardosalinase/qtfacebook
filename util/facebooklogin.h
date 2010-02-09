#ifndef FACEBOOKLOGIN_H
#define FACEBOOKLOGIN_H

#include <QNetworkAccessManager>

#include "userinfo.h"

namespace UTIL {

class FacebookLogin : public QObject
{
    Q_OBJECT

public:
    FacebookLogin(UserInfo *userInfo, QObject *parent = 0);
    void logIn();

signals:
    void loginResults(bool);

private slots:
    void gotNetworkReply(QNetworkReply *reply);

private:
    UserInfo *m_userInfo;
    QNetworkAccessManager *m_nam;
    int m_count;


};

} // namespace UTIL

#endif // FACEBOOKLOGIN_H
