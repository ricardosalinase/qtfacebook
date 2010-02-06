#include "fql_GetAppInfo.h"

#include <QDebug>

namespace API {
namespace FQL {

GetAppInfo::GetAppInfo(QObject *parent) :
        API::Method(parent),
        m_currentAppInfo(0)
{
    m_appInfo = new QMap<QString, AppInfo *>;

    requires("query");
}

bool GetAppInfo::prepare() {

    QString fql = "SELECT app_id, icon_url FROM application WHERE app_id IN (";
    fql.append(m_argMap.take("app_ids").toStringList().join(","));
    fql.append(")");

    m_argMap.insert("query", fql);

    return true;

}

bool GetAppInfo::startElement(const QString & /* namespaceURI */,
                           const QString & /* localName */,
                           const QString &qName,
                           const QXmlAttributes &/*attributes*/) {
    qDebug() << "Start: " << qName;
    if (qName == "app_info")
        m_currentAppInfo = new AppInfo();

    m_currentText.clear();
    return true;
}

bool GetAppInfo::endElement(const QString &/*namespaceURI*/,
                         const QString &/*localName*/,
                         const QString &qName) {

    if (m_currentAppInfo) {

        if (qName == "app_id")
            m_currentAppInfo->setAppId(m_currentText);
        else if (qName == "api_key")
            m_currentAppInfo->setApiKey(m_currentText);
        else if (qName == "canvas_name")
            m_currentAppInfo->setCanvasName(m_currentText);
        else if (qName == "display_name")
            m_currentAppInfo->setDisplayName(m_currentText);
        else if (qName == "icon_url")
            m_currentAppInfo->setIconUrl(m_currentText);
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
            m_appInfo->insert(m_currentAppInfo->getAppId(), m_currentAppInfo);
            m_currentAppInfo = 0;
        }
    }

    qDebug() << "End: " << qName << " " <<  m_currentText;
    return true;

}
QString GetAppInfo::getMethodName() {
    return "fql.query.getAppInfo";
}

QMap<QString, AppInfo*> * GetAppInfo::getAppInfoMap() {
    return m_appInfo;
}


} // namespace FQL
} // namespace API
