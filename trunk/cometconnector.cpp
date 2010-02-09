#include <QNetworkRequest>
#include <QVariantMap>
#include <QVariantList>
#include <QDebug>

#include "cometconnector.h"
#include "util/cookiejar.h"



CometConnector::CometConnector(UserInfo *userInfo, QObject *parent) :
    QThread(parent)
{
    m_userInfo = userInfo;
}

void CometConnector::run() {

    CometConnection *cc = new CometConnection(m_userInfo);
    connect(cc, SIGNAL(newNotification(DATA::Notification*,DATA::AppInfo*)),
            this, SIGNAL(newNotification(DATA::Notification*, DATA::AppInfo *)),
            Qt::QueuedConnection);
    connect(cc, SIGNAL(notificationAck(QString)),
            this, SIGNAL(notificationAck(QString)),
            Qt::QueuedConnection);
    cc->go();




    exec();
}

CometConnection::CometConnection(UserInfo *userInfo, QObject *parent) :
        QObject(parent),
        m_userInfo(userInfo)
{

}


void CometConnection::go() {

    m_cometString = "http://0.channel" + m_userInfo->getChannel() +
                 ".facebook.com/x/0/false/p_" + m_userInfo->getUID() +
                 "=";

    m_parser = new QJson::Parser();

    m_cometNam = new QNetworkAccessManager();
    m_nam = new QNetworkAccessManager();

    // The cookiejar can't live in another thread, and you can't
    // copy QObject derived classes ...
    UTIL::CookieJar *cj = new UTIL::CookieJar();
    cj->createJarFromRaw(m_userInfo->getCookieJar()->getRawCookies());

    m_cometNam->setCookieJar(cj);
    cj->setParent(0);
    m_nam->setCookieJar(cj);



    connect(m_cometNam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotCometMessage(QNetworkReply*)));

    // First thing we do is connect to the comet server and find our sequence number
    QNetworkRequest nr;
    QUrl url(m_cometString + "-1");

    qDebug() << url;
    nr.setUrl(url);
    m_cometNam->get(nr);

}

void CometConnection::gotCometMessage(QNetworkReply *reply) {

    QByteArray result;

    if (reply->error() > 0) {
        qDebug() << "Error number = " << reply->errorString();
    }
    else
    {
        result = reply->readAll();
        qDebug() << result;

        // remove 'for (;;);' from the beginning
        result.remove(0,9);

        QVariantMap topLevel = m_parser->parse(result).toMap();

        if (topLevel["t"].toString().compare("refresh") == 0)
        {
            m_seq = topLevel["seq"].toInt();
        }
        else if (topLevel["t"].toString().compare("continue") == 0)
        {
            // no op
        }
        else if (topLevel["t"].toString().compare("msg") == 0)
        {
            QVariantList ms = topLevel["ms"].toList();

            for (int i = 0; i < ms.size(); i++) {

                QVariantMap msEntry = ms.at(i).toMap();

                if (msEntry["type"].toString().compare("app_msg") == 0) {

                    QVariantMap payload = (msEntry["response"].toMap())["payload"].toMap();
                    DATA::Notification *n = new DATA::Notification();
                    n->setTitleHtml(payload["title"].toString());
                    n->setIsRead(!(payload["alert"].toMap())["unread"].toBool());
                    n->setNotificationId(payload["alertId"].toString());
                    n->setCreatedTime((payload["alert"].toMap())["time_sent"].toString());
                    n->setAppId((payload["alert"].toMap())["app_id"].toString());

                    DATA::AppInfo *a = new DATA::AppInfo();
                    a->setAppId(n->getAppId());
                    a->setIconUrl(payload["icon"].toString());

                    emit newNotification(n,a);

                } else if (msEntry["type"].toString().compare("notifications_read") == 0) {

                    // {"t":"msg","c":"p_100000685644751","ms":[{"alert_ids":["314426"],"num_unread":3,"type":"notifications_read"}]}
                    QVariantList alertIds = msEntry["alert_ids"].toList();
                    while (!alertIds.empty()) {
                        QString alertId = alertIds.takeFirst().toString();
                        emit notificationAck(alertId);
                    }


                }


            }

            m_seq++;
        }

        QNetworkRequest nr;
        QUrl url(m_cometString + QString::number(m_seq));
        nr.setUrl(url);
        m_cometNam->get(nr);


    }

}
