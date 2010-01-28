#include "notifications_getlist.h"
#include <QDebug>

namespace API {
namespace Notifications {

GetList::GetList(QObject *parent) : Method(parent), m_currentNotification(0)
{
    m_notifications = new QList<Notification*>;

}

bool GetList::startElement(const QString & /* namespaceURI */,
                           const QString & /* localName */,
                           const QString &qName,
                           const QXmlAttributes &attributes) {
    qDebug() << "Start: " << qName;
    if (qName == "notification")
        m_currentPerson = new Person();
    else if (qName == "app_info")
        m_currentApp = new App();

    m_currentText.clear();
    return true;
}

bool GetList::endElement(const QString &namespaceURI,
                         const QString &localName,
                         const QString &qName) {


    if (qName == "notification_id")
        m_currentPerson->setNotificationId(m_currentText);
    else if (qName == "sender_id")
        m_currentPerson->setSenderId(m_currentText);
    else if (qName == "recipient_id")
        m_currentPerson->setRecipientId(m_currentText);
    else if (qName == "created_time")
        m_currentPerson->setCreatedTime(m_currentText);
    else if (qName == "updated_time")
        m_currentPerson->setUpdatedTime(m_currentText);
    else if (qName == "title_html")
        m_currentPerson->setTitleHtml(m_currentText);
    else if (qName == "title_text")
        m_currentPerson->setTitleText(m_currentText);
    else if (qName == "body_html")
        m_currentPerson->setBodyHtml(m_currentText);
    else if (qName == "body_text")
        m_currentPerson->setBodyText(m_currentText);
    else if (qName == "href")
        m_currentPerson->setHref(m_currentText);
    else if (qName == "app_id")
        m_currentPerson->setAppId(m_currentText);
    else if (qName == "is_unread")
        m_currentPerson->setIsRead(m_currentText.compare("1") ? true : false);
    else if (qName == "is_hidden")
        m_currentPerson->setIsHidden(m_currentText.compare("1") ? true : false);
    else if (qName == "notification")
        m_notifications->append(m_currentPerson);
    else if (qName == "app_info")
        m_notifications->append(m_currentApp);


    qDebug() << "End: " << qName << " " <<  m_currentText;


    return true;
}

QString GetList::getMethodName() {
    return "notifications.getList";
}

QList<Notification *> * GetList::getNotifications() {
    return m_notifications;
}

} // namespace Notifications
} // namespace API
