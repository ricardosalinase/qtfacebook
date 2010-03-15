#include "FbChatConnection.h"

#include <QDebug>

#include "util/OurUserInfo.h"

FbChatConnection::FbChatConnection(QObject *parent) :
    Worker(parent)
{

}

FbChatConnection::init()
{

    QXmppConfiguration config;
    config.setHost("chat.facebook.com");
    config.setDomain("chat.facebook.com");
    config.setSASLAuthMechanism(QXmppConfiguration::SASLFacebookPlatform);

    UTIL::OurUserInfo *info = UTIL::OurUserInfo::getInstance();

    config.setFacebookApiKey(info->getApiKey());
    config.setFacebookSessionKey(info->getSessionKey());
    config.setFacebookSecretKey(info->getSecret());

    this->connectToServer(config);

    QXmppVCardManager &vCardManager = getVCardManager();
    connect(&vCardManager, SIGNAL(vCardReceived(QXmppVCard)),
            this, SLOT(vCardReceived(QXmppVCard)));

}

FbChatConnection::~FbChatConnection()
{

}

void FbChatConnection::presenceReceived(QXmppPresence presence)
{
    qDebug() << "Presence Received: " << presence.getFrom() <<
            " Status Type: " << statusTypeString(presence.getStatus().getType()) <<
            " Status TypeStr: " << presence.getStatus().getTypeStr() <<
            " Status Text: " << presence.getStatus().getStatusText() <<
            " Presence Type: " << presenceTypeString(presence.getType());

    QXmppElementList el = presence.extensions();
    for (int i = 0; i < el.size(); i++)
        qDebug() << "extension: " << el.at(i)

    //getVCardManager().requestVCard(presence.getFrom());

}

QString FbChatConnection::statusTypeString(QXmppPresence::Status::Type type) {

    switch(type)
    {
    case QXmppPresence::Status::Offline:
        return "Offline";
    case QXmppPresence::Status::Online:
        return "Online";
    case QXmppPresence::Status::Away:
        return "Away";
    case QXmppPresence::Status::XA:
        return "XA";
    case QXmppPresence::Status::DND:
        return "DND";
    case QXmppPresence::Status::Chat:
        return "Chat";
    case QXmppPresence::Status::Invisible:
        return "Invisible";
    default:
        return "Unknown Status Type";
    }

}

QString FbChatConnection::presenceTypeString(QXmppPresence::Type type) {
    QString text;
    switch(type)
    {
    case QXmppPresence::Error:
        text = "error";
        break;
    case QXmppPresence::Available:
        text = "available";
        break;
    case QXmppPresence::Unavailable:
        text = "unavailable";
        break;
    case QXmppPresence::Subscribe:
        text = "subscribe";
        break;
    case QXmppPresence::Subscribed:
        text = "subscribed";
        break;
    case QXmppPresence::Unsubscribe:
        text = "unsubscribe";
        break;
    case QXmppPresence::Unsubscribed:
        text = "unsubscribed";
        break;
    case QXmppPresence::Probe:
        text = "probe";
        break;
    default:
        qWarning("QXmppPresence::getTypeStr() invalid type %d", (int)type);
        break;
    }
    return text;
}

void FbChatConnection::vCardReceived(QXmppVCard vCard) {

    qDebug() << "vCard; " << vCard.fullName();
    qDebug() << "Photo: " << vCard.photo().size();



}

