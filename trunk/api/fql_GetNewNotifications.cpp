#include "fql_GetNewNotifications.h"

#include <QDebug>

namespace API {
namespace FQL {

GetNewNotifications::GetNewNotifications(QObject *parent) :
    API::Method(parent),
    m_currentNotification(0)
{
    m_notifications = new QList<Notification*>;
    m_appIds = new QStringList();

    requires("query");

}

bool GetNewNotifications::prepare() {

    // This is called prior to validate() so you're able to set up the query

    QString fql("SELECT notification_id, created_time, title_html, app_id, is_unread, is_hidden  FROM notification WHERE recipient_id=");
    fql.append(m_userInfo->getUID());
    fql.append(" AND created_time > ");
    fql.append(m_argMap.take("start_time").toString());

    m_argMap.insert("query", fql);

    //&query=SELECT notification_id, created_time, title_html, app_id, is_unread, is_hidden  FROM notification WHERE recipient_id=1082239928 AND created_time > 1265157482
    // uint time1 = QDateTime::currentDateTime().toUTC().toTime_t()

    return true;

}

bool GetNewNotifications::startElement(const QString & /* namespaceURI */,
                           const QString & /* localName */,
                           const QString &qName,
                           const QXmlAttributes &attributes) {
    qDebug() << "Start: " << qName;
    if (qName == "notification")
        m_currentNotification = new Notification();


    m_currentText.clear();
    return true;
}

bool GetNewNotifications::endElement(const QString &namespaceURI,
                         const QString &localName,
                         const QString &qName) {


    if (m_currentNotification)
    {

        if (qName == "notification_id")
            m_currentNotification->setNotificationId(m_currentText);
        //else if (qName == "sender_id")
            //m_currentNotification->setSenderId(m_currentText);
        //else if (qName == "recipient_id")
            //m_currentNotification->setRecipientId(m_currentText);
        else if (qName == "created_time")
            m_currentNotification->setCreatedTime(m_currentText);
        //else if (qName == "updated_time")
            //m_currentNotification->setUpdatedTime(m_currentText);
        else if (qName == "title_html") {
            fixHtml(&m_currentText);
            m_currentNotification->setTitleHtml(m_currentText);
        }
        //else if (qName == "title_text")
            //m_currentNotification->setTitleText(m_currentText);
        //else if (qName == "body_html")
            //m_currentNotification->setBodyHtml(m_currentText);
        //else if (qName == "body_text")
            //m_currentNotification->setBodyText(m_currentText);
        //else if (qName == "href")
            //m_currentNotification->setHref(m_currentText);
        else if (qName == "app_id") {
            m_appIds->append(m_currentText);
            m_currentNotification->setAppId(m_currentText);
        }
        else if (qName == "is_unread")
            m_currentNotification->setIsRead((m_currentText.compare("1") == 0) ? false : true);
        else if (qName == "is_hidden")
            m_currentNotification->setIsHidden((m_currentText.compare("1") == 0) ? true : false);
        else if (qName == "notification") {
            m_notifications->append(m_currentNotification);
            m_currentNotification = 0;
        }

    }

    qDebug() << "End: " << qName << " " <<  m_currentText;


    return true;
}

// The HTML is escaped, but it's fairly limited
void GetNewNotifications::fixHtml(QString *html) {

    html->replace("&gt;",">");
    html->replace("&lt;", "<");
    html->replace("&quot;","\"");
    html->replace("&amp;","&");
    html->replace("#039;","'");

}

QString GetNewNotifications::getMethodName() {
    return "fql.query.getNewNotifications";
}

QList<Notification*> * GetNewNotifications::getNotificationList() {
    return m_notifications;
}

QStringList * GetNewNotifications::getAppIds() {
    return m_appIds;
}

} // namespace FQL
} // namespace API
