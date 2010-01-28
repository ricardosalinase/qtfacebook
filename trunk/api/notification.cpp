#include "notification.h"
namespace API {
namespace Notifications {

Notification::Notification()
{

}

Notification::type Notification::getType() {
    return m_type;
}

void Notification::setType(Notification::type t) {
    m_type = t;
}

Person::Person() {
    setType(PERSON);
}

void Person::setNotificationId(QString nid) {
    m_notificationId = nid;
}

QString Person::getNotificaitonId() {
    return m_notificationId;
}

void Person::setSenderId(QString sid) {
    m_senderId = sid;
}

QString Person::getSenderId() {
    return m_senderId;
}

void Person::setRecipientId(QString rid) {
    m_recipientId = rid;
}

QString Person::getRecipientId() {
    return m_recipientId;
}

void Person::setCreatedTime(QString ctime) {
    m_createdTime = ctime;
}

QString Person::getCreatedTime() {
    return m_createdTime;
}

void Person::setUpdatedTime(QString utime) {
    m_updatedTime = utime;
}

QString Person::getUpdatedTime() {
    return m_updatedTime;
}

void Person::setTitleHtml(QString html) {
    m_titleHtml = html;
}

QString Person::getTitleHtml() {
    return m_titleHtml;
}

void Person::setTitleText(QString text) {
    m_titleText = text;
}

QString Person::getTitleText() {
    return m_titleText;
}

void Person::setBodyHtml(QString html) {
    m_bodyHtml = html;
}

QString Person::getBodyHtml() {
    return m_bodyHtml;
}

void Person::setBodyText(QString text) {
    m_bodyText = text;
}

QString Person::getBodyText() {
    return m_bodyText;
}

void Person::setHref(QString href) {
    m_href = href;
}

QString Person::getHref() {
    return m_href;
}

void Person::setAppId(QString aid) {
    m_appId = aid;
}

QString Person::getAppId() {
    return m_appId;
}

void Person::setIsRead(bool ir) {
    m_isRead = ir;
}

bool Person::getIsRead() {
    return m_isRead;
}

void Person::setIsHidden(bool ih) {
    m_isHidden = ih;
}

bool Person::getIsHidden() {
    return m_isHidden;
}

App::App() {
    setType(APP);
}

void App::setAppId(QString appId) {
    m_appId = appId;
}

QString App::getAppId() {
    return m_appId;
}

void App::setApiKey(QString apiKey) {
    m_apiKey = apiKey;
}

QString App::getApiKey() {
    return m_apiKey;
}

void App::setDisplayName(QString dn) {
    m_displayName = dn;
}

QString App::getDisplayName() {
    return m_displayName;
}

void App::setIconUrl(QString iu) {
    m_iconUrl = iu;
}

QString App::getIconUrl() {
    return m_iconUrl;
}

void App::setLogoUrl(QString lu) {
    m_logoUrl = lu;
}

QString App::getLogoUrl() {
    return m_logoUrl;
}

void App::setCompanyName(QString cn) {
    m_companyName = cn;
}

QString App::getCompanyName() {
    return m_companyName;
}

void App::setDescription(QString d) {
    m_description = d;
}

QString App::getDescription() {
    return m_description;
}

void App::setDailyActiveUsers(QString dau) {
    m_dailyActiveUsers = dau;
}

QString App::getDailyActiveUsers() {
    return m_dailyActiveUsers;
}

void App::setWeeklyActiveUsers(QString wau) {
    m_weeklyActiveUsers = wau;
}

QString App::getWeeklyActiveUsers() {
    return m_weeklyActiveUsers;
}

void App::setMonthlyActiveUsers(QString mau) {
    m_monthlyActiveUsers = mau;
}

QString App::getMonthlyActiveUsers() {
    return m_monthlyActiveUsers;
}

void App::setCategory(QString c) {
    m_category = c;
}

QString App::getCategory() {
    return m_category;
}

void App::setSubCategory(QString sc) {
    m_subCategory = sc;
}

QString App::getSubCategory() {
    return m_subCategory;
}

}
}
