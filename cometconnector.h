#ifndef COMETCONNECTOR_H
#define COMETCONNECTOR_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <qjson/parser.h>

#include "userinfo.h"
#include "data/notification.h"

class CometConnector : public QThread
{
Q_OBJECT
public:
    explicit CometConnector(UserInfo *userInfo, QObject *parent = 0);

signals:
    void newNotification(DATA::Notification *);
    void newChatMessage();



protected:
    void run();

private:
    UserInfo *m_userInfo;

};

class CometConnection : public QObject
{
    Q_OBJECT
public:
    explicit CometConnection(UserInfo *userInfo, QObject *parent = 0);
    void go();

signals:
    void newNotification(DATA::Notification *);
    void newChatMessage();

private slots:
    void gotCometMessage(QNetworkReply *reply);

private:
    UserInfo *m_userInfo;
    QNetworkAccessManager *m_cometNam;
    QNetworkAccessManager *m_nam;
    QString m_cometString;
    QJson::Parser *m_parser;
    int m_seq;



};

#endif // COMETCONNECTOR_H
