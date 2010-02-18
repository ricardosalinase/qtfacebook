#include "fql_GetNewNotifications.h"

#include <QDebug>

namespace API {
namespace FQL {

GetNewNotifications::GetNewNotifications(QObject *parent) :
    API::Method(parent),
    m_currentNotification(0),
    m_currentAppInfo(0),
    m_parseState(QUERY)
{
    m_notifications = new QList<Notification*>;
    m_appIds = new QStringList();

    requires("queries");

}

bool GetNewNotifications::prepare() {

    // This is called prior to validate() so you're able to set up the query

    QString fql("{\"notifications\":\"SELECT notification_id, created_time, title_html, title_text, app_id, "
                  "is_unread, is_hidden  FROM notification WHERE recipient_id=");
    fql.append(m_userInfo->getUID());
    fql.append(" AND created_time > ");
    fql.append(m_argMap.take("start_time").toString());

    if (m_argMap.contains("only_unread")) {
        m_argMap.take("only_unread");
        fql.append(" AND is_unread = 1");
    }

    if (m_argMap.contains("only_read")) {
        m_argMap.take("only_read");
        fql.append(" AND is_unread = 0");
    }

    if (m_argMap.contains("get_hidden"))
        m_argMap.take("get_hidden");
    else
        fql.append(" AND is_hidden = 0");

    if (m_argMap.contains("limit")) {
        QString limit = m_argMap.take("limit").toString();
        fql.append(" LIMIT " + limit);
    }

    fql.append("\",\"app_info\":\"SELECT app_id, icon_url FROM application WHERE app_id IN (SELECT app_id FROM #notifications)\"}");

    m_argMap.insert("queries", fql);

    //&query=SELECT notification_id, created_time, title_html, app_id, is_unread, is_hidden  FROM notification WHERE recipient_id=1082239928 AND created_time > 1265157482
    // uint time1 = QDateTime::currentDateTime().toUTC().toTime_t()

    //qDebug() << "Query: " << fql;

    return true;

}

bool GetNewNotifications::startElement(const QString & /* namespaceURI */,
                           const QString & /* localName */,
                           const QString &qName,
                           const QXmlAttributes &/*attributes*/) {
    qDebug() << "Start: " << qName;

    if (m_parseState != QUERY)
    {
        if (qName == "notification")
            m_currentNotification = new Notification();
        else if (qName == "app_info")
            m_currentAppInfo = new AppInfo();
    }

    m_currentText.clear();
    return true;
}

bool GetNewNotifications::endElement(const QString &/*namespaceURI*/,
                         const QString &/*localName*/,
                         const QString &qName) {

    switch(m_parseState)
    {
    case QUERY:
        if (qName == "name")
        {
            if (m_currentText == "notifications")
                m_parseState = NOTIFICATION;
            else if (m_currentText == "app_info")
                m_parseState = APPINFO;
        }
        break;
    case NOTIFICATION:

        if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "notification_id")
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
        else if (qName == "title_text")
            m_currentNotification->setTitleText(m_currentText);
        //else if (qName == "body_html")
            //m_currentNotification->setBodyHtml(m_currentText);
        //else if (qName == "body_text")
            //m_currentNotification->setBodyText(m_currentText);
        //else if (qName == "href")
            //m_currentNotification->setHref(m_currentText);
        else if (qName == "app_id")
            m_currentNotification->setAppId(m_currentText);
        else if (qName == "is_unread")
            m_currentNotification->setIsRead((m_currentText.compare("1") == 0) ? false : true);
        else if (qName == "is_hidden")
            m_currentNotification->setIsHidden((m_currentText.compare("1") == 0) ? true : false);
        else if (qName == "notification") {
            m_notifications->append(m_currentNotification);
            m_tmpMap.insertMulti(m_currentNotification->getAppId(), m_currentNotification);
            m_currentNotification = 0;
        }


        break;
    case APPINFO:
        if (qName == "fql_result")
            m_parseState = QUERY;
        else if (qName == "app_id")
            m_currentAppInfo->setAppId(m_currentText);
        else if (qName == "api_key")
            m_currentAppInfo->setApiKey(m_currentText);
        else if (qName == "canvas_name")
            m_currentAppInfo->setCanvasName(m_currentText);
        else if (qName == "display_name")
            m_currentAppInfo->setDisplayName(m_currentText);
        else if (qName == "icon_url")
        {
            m_currentAppInfo->setIconUrl(m_currentText);
            qDebug() << m_currentText;
        }
        else if (qName == "logo_url")
            m_currentAppInfo->setLogoUrl(m_currentText);
        else if (qName == "company_name")
            m_currentAppInfo->setCompanyName(m_currentText);
        else if (qName == "description")
            m_currentAppInfo->setDescription(m_currentText);
        else if (qName == "daily_active_users")
            m_currentAppInfo->setDailyActiveUsers(m_currentText);
        else if (qName == "weekly_active_users")
            m_currentAppInfo->setWeeklyActiveUsers(m_currentText);
        else if (qName == "monthly_active_users")
            m_currentAppInfo->setMonthlyActiveUsers(m_currentText);
        else if (qName == "category")
            m_currentAppInfo->setCategory(m_currentText);
        else if (qName == "subcategory")
            m_currentAppInfo->setSubCategory(m_currentText);
        else if (qName == "app_info") {
            QList<Notification *> nList = m_tmpMap.values(m_currentAppInfo->getAppId());
            for (int i = 0; i < nList.size(); i++)
            {
                nList.at(i)->setAppInfo(m_currentAppInfo);
            }
            delete m_currentAppInfo;
            m_currentAppInfo = 0;
        }
        break;

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
    return "fql.multiquery.getNewNotifications";
}

QList<Notification*> * GetNewNotifications::getNotificationList() {
    return m_notifications;
}

QStringList * GetNewNotifications::getAppIds() {
    return m_appIds;
}

} // namespace FQL
} // namespace API
