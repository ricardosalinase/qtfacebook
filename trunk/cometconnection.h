#ifndef COMETCONNECTION_H
#define COMETCONNECTION_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <QList>

#include <qjson/parser.h>

#include "userinfo.h"
#include "data/notification.h"
#include "data/appinfo.h"
#include "data/chatmessage.h"
#include "data/buddy.h"
#include "util/worker.h"

class CometConnection : public UTIL::Worker
{
    Q_OBJECT
public:
    explicit CometConnection(UserInfo *userInfo, QObject *parent = 0);
    void init();

signals:
    void newNotification(DATA::Notification *, DATA::AppInfo *);
    void notificationAck(QString nid);
    void newChatMessage(DATA::ChatMessage *);
    void newBuddyList(QList<DATA::Buddy*> *, QMap<QString, QString> *);

public slots:
    void sendChatMessage(DATA::ChatMessage *);
    void getBuddyList();

private slots:
    void gotCometMessage(QNetworkReply *reply);
    void gotNetworkReply(QNetworkReply *reply);

private:
    UserInfo *m_userInfo;
    QNetworkAccessManager *m_cometNam;
    QNetworkAccessManager *m_nam;
    QString m_cometString;
    QJson::Parser *m_parser;
    int m_seq;



};


#endif // COMETCONNECTION_H
