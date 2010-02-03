#include "appinfo.h"

namespace DATA {

AppInfo::AppInfo()
{
    m_iconPixmap = 0;
    m_logoPixmap = 0;
}

AppInfo::~AppInfo() {
    if (m_iconPixmap != 0)
        delete m_iconPixmap;
    if (m_logoPixmap != 0)
        delete m_logoPixmap;
}

void AppInfo::setAppId(QString appId) {
    m_appId = appId;
}

QString AppInfo::getAppId() {
    return m_appId;
}

void AppInfo::setApiKey(QString apiKey) {
    m_apiKey = apiKey;
}

QString AppInfo::getApiKey() {
    return m_apiKey;
}

void AppInfo::setDisplayName(QString dn) {
    m_displayName = dn;
}

QString AppInfo::getDisplayName() {
    return m_displayName;
}

void AppInfo::setIconUrl(QString iu) {
    m_iconUrl = iu;
}

QString AppInfo::getIconUrl() {
    return m_iconUrl;
}

void AppInfo::setLogoUrl(QString lu) {
    m_logoUrl = lu;
}

QString AppInfo::getLogoUrl() {
    return m_logoUrl;
}

void AppInfo::setCompanyName(QString cn) {
    m_companyName = cn;
}

QString AppInfo::getCompanyName() {
    return m_companyName;
}

void AppInfo::setDescription(QString d) {
    m_description = d;
}

QString AppInfo::getDescription() {
    return m_description;
}

void AppInfo::setDailyActiveUsers(QString dau) {
    m_dailyActiveUsers = dau;
}

QString AppInfo::getDailyActiveUsers() {
    return m_dailyActiveUsers;
}

void AppInfo::setWeeklyActiveUsers(QString wau) {
    m_weeklyActiveUsers = wau;
}

QString AppInfo::getWeeklyActiveUsers() {
    return m_weeklyActiveUsers;
}

void AppInfo::setMonthlyActiveUsers(QString mau) {
    m_monthlyActiveUsers = mau;
}

QString AppInfo::getMonthlyActiveUsers() {
    return m_monthlyActiveUsers;
}

void AppInfo::setCategory(QString c) {
    m_category = c;
}

QString AppInfo::getCategory() {
    return m_category;
}

void AppInfo::setSubCategory(QString sc) {
    m_subCategory = sc;
}

QString AppInfo::getSubCategory() {
    return m_subCategory;
}

void AppInfo::setCanvasName(QString cn) {
    m_canvasName = cn;
}

QString AppInfo::getCanvasName() {
    return m_canvasName;
}

void AppInfo::setLogoPixmap(QPixmap *p) {
    m_logoPixmap = new QPixmap(*p);
}

QPixmap * AppInfo::getLogoPixmap() {
    if (m_logoPixmap == 0)
        return new QPixmap();
    else
        return new QPixmap(*m_logoPixmap);
}

void AppInfo::setIconPixmap(QPixmap *p) {
    m_iconPixmap = new QPixmap(*p);
}

QPixmap * AppInfo::getIconPixmap() {
    if (m_iconPixmap == 0)
        return new QPixmap();
    else
        return new QPixmap(*m_iconPixmap);
}

} // namespace DATA
