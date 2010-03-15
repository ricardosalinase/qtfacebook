#ifndef FBCHATCONNECTION_H
#define FBCHATCONNECTION_H

#include <QObject>

#include "QXmppClient.h"
#include "QXmppVCard.h"


#include "util/worker.h"
#include "api/factory.h"


class FbChatConnection : public UTIL::Worker, public QXmppClient
{
Q_OBJECT
public:
    explicit FbChatConnection(QObject *parent = 0);
    ~FbChatConnection();

    void init();

private slots:
    void presenceReceived(QXmppPresence);
    void vCardReceived(QXmppVCard);


signals:

public slots:

private:
    API::Factory *m_factory;
    QString statusTypeString(QXmppPresence::Status::Type type);
    QString presenceTypeString(QXmppPresence::Type type);

};

#endif // FBCHATCONNECTION_H
