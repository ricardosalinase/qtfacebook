#include "notifications_getlist.h"
#include <QDebug>

namespace API {
namespace Notifications {

GetList::GetList(QObject *parent) : Method(parent),
    m_currentNotification(0),
    m_currentAppInfo(0)
{
    m_notifications = new QList<Notification*>;
    m_appInfo = new QList<AppInfo *>;

}

bool GetList::startElement(const QString & /* namespaceURI */,
                           const QString & /* localName */,
                           const QString &qName,
                           const QXmlAttributes &attributes) {
    qDebug() << "Start: " << qName;
    if (qName == "notification")
        m_currentNotification = new Notification();
    else if (qName == "app_info")
        m_currentAppInfo = new AppInfo();

    m_currentText.clear();
    return true;
}

bool GetList::endElement(const QString &namespaceURI,
                         const QString &localName,
                         const QString &qName) {


    if (m_currentNotification)
    {

        if (qName == "notification_id")
            m_currentNotification->setNotificationId(m_currentText);
        else if (qName == "sender_id")
            m_currentNotification->setSenderId(m_currentText);
        else if (qName == "recipient_id")
            m_currentNotification->setRecipientId(m_currentText);
        else if (qName == "created_time")
            m_currentNotification->setCreatedTime(m_currentText);
        else if (qName == "updated_time")
            m_currentNotification->setUpdatedTime(m_currentText);
        else if (qName == "title_html")
            m_currentNotification->setTitleHtml(m_currentText);
        else if (qName == "title_text")
            m_currentNotification->setTitleText(m_currentText);
        else if (qName == "body_html")
            m_currentNotification->setBodyHtml(m_currentText);
        else if (qName == "body_text")
            m_currentNotification->setBodyText(m_currentText);
        else if (qName == "href")
            m_currentNotification->setHref(m_currentText);
        else if (qName == "app_id")
            m_currentNotification->setAppId(m_currentText);
        else if (qName == "is_unread")
            m_currentNotification->setIsRead((m_currentText.compare("1") == 0) ? true : false);
        else if (qName == "is_hidden")
            m_currentNotification->setIsHidden((m_currentText.compare("1") == 0) ? true : false);
        else if (qName == "notification") {
            m_notifications->append(m_currentNotification);
            m_currentNotification = 0;
        }

    } else if (m_currentAppInfo) {

        if (qName == "app_id")
            m_currentAppInfo->setAppId(qName);
        else if (qName == "api_key")
            m_currentAppInfo->setApiKey(qName);
        else if (qName == "canvas_name")
            m_currentAppInfo->setCanvasName(qName);
        else if (qName == "display_name")
            m_currentAppInfo->setDisplayName(qName);
        else if (qName == "icon_url")
            m_currentAppInfo->setIconUrl(qName);
        else if (qName == "logo_url")
            m_currentAppInfo->setLogoUrl(qName);
        else if (qName == "company_name")
            m_currentAppInfo->setCompanyName(qName);
        else if (qName == "description")
            m_currentAppInfo->setDescription(qName);
        else if (qName == "daily_active_users")
            m_currentAppInfo->setDailyActiveUsers(qName);
        else if (qName == "weekly_active_users")
            m_currentAppInfo->setWeeklyActiveUsers(qName);
        else if (qName == "monthly_active_users")
            m_currentAppInfo->setMonthlyActiveUsers(qName);
        else if (qName == "category")
            m_currentAppInfo->setCategory(qName);
        else if (qName == "subcategory")
            m_currentAppInfo->setSubCategory(qName);
        else if (qName == "app_info") {
            m_appInfo->append(m_currentAppInfo);
            m_currentAppInfo = 0;
        }
    }

    qDebug() << "End: " << qName << " " <<  m_currentText;


    return true;
}

QString GetList::getMethodName() {
    return "notifications.getList";
}

QList<Notification *> * GetList::getNotifications() {
    return m_notifications;
}

QList<AppInfo *> * GetList::getAppInfo() {
    return m_appInfo;
}

} // namespace Notifications
} // namespace API
