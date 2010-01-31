#include "notification.h"
#include <QUrl>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QDebug>

namespace API {
namespace Notifications {

Notification::Notification()
{




}

void Notification::setNotificationId(QString nid) {
    m_notificationId = nid;
}

QString Notification::getNotificationId() {
    return m_notificationId;
}

void Notification::setSenderId(QString sid) {
    m_senderId = sid;
}

QString Notification::getSenderId() {
    return m_senderId;
}

void Notification::setRecipientId(QString rid) {
    m_recipientId = rid;
}

QString Notification::getRecipientId() {
    return m_recipientId;
}

void Notification::setCreatedTime(QString ctime) {
    m_createdTime = ctime;
}

QString Notification::getCreatedTime() {
    return m_createdTime;
}

void Notification::setUpdatedTime(QString utime) {
    m_updatedTime = utime;
}

QString Notification::getUpdatedTime() {
    return m_updatedTime;
}

void Notification::setTitleHtml(QString html) {
    m_titleHtml = html;
}

QString Notification::getTitleHtml() {
    return m_titleHtml;
}

void Notification::setTitleText(QString text) {
    m_titleText = text;
}

QString Notification::getTitleText() {
    return m_titleText;
}

void Notification::setBodyHtml(QString html) {
    m_bodyHtml = html;
}

QString Notification::getBodyHtml() {
    return m_bodyHtml;
}

void Notification::setBodyText(QString text) {
    m_bodyText = text;
}

QString Notification::getBodyText() {
    return m_bodyText;
}

void Notification::setHref(QString href) {
    m_href = href;
}

QString Notification::getHref() {
    return m_href;
}

void Notification::setAppId(QString aid) {
    m_appId = aid;
}

QString Notification::getAppId() {
    return m_appId;
}

void Notification::setIsRead(bool ir) {
    m_isRead = ir;
}

bool Notification::getIsRead() {
    return m_isRead;
}

void Notification::setIsHidden(bool ih) {
    m_isHidden = ih;
}

bool Notification::getIsHidden() {
    return m_isHidden;
}

AppInfo::AppInfo(QObject *parent) :
        QObject(parent),
        m_iconReply(0),
        m_logoReply(0)
{
    m_nam = new QNetworkAccessManager();
    QObject::connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotReply(QNetworkReply*)));
    QNetworkDiskCache *diskCache = new QNetworkDiskCache();
    QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    diskCache->setCacheDirectory(location);
    m_nam->setCache(diskCache);

    m_iconPixmap = 0;
    m_logoPixmap = 0;

}



AppInfo * AppInfo::clone()
{
    AppInfo *ai = new AppInfo();

    ai->setAppId(m_appId);
    ai->setApiKey(m_apiKey);
    ai->setDisplayName(m_displayName);
    ai->setIconUrl(m_iconUrl, false);
    ai->setLogoUrl(m_logoUrl, false);
    ai->setCompanyName(m_companyName);
    ai->setDescription(m_description);
    ai->setDailyActiveUsers(m_dailyActiveUsers);
    ai->setWeeklyActiveUsers(m_weeklyActiveUsers);
    ai->setMonthlyActiveUsers(m_monthlyActiveUsers);
    ai->setCategory(m_category);
    ai->setSubCategory(m_subCategory);
    ai->setCanvasName(m_canvasName);
    ai->setIconPixmap(m_iconPixmap);
    ai->setLogoPixmap(m_logoPixmap);

    return ai;

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

void AppInfo::setIconUrl(QString iu, bool fetchPixmap) {
    m_iconUrl = iu;

    if (fetchPixmap) {
        QUrl url(m_iconUrl);
        QNetworkReply* reply = m_nam->get(QNetworkRequest(url));
        m_iconReply = reply;
    }
}

QString AppInfo::getIconUrl() {
    return m_iconUrl;
}

void AppInfo::setLogoUrl(QString lu, bool fetchPixmap) {
    m_logoUrl = lu;

    if (fetchPixmap) {
        QUrl url(m_logoUrl);
        QNetworkReply* reply = m_nam->get(QNetworkRequest(url));
        m_logoReply = reply;

    }
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
    m_logoPixmap = p;
}

QPixmap * AppInfo::getLogoPixmap() {
    if (m_logoPixmap == 0) {
        qDebug() << "Created blank pixmap";
       return new QPixmap();
   }
   else
       return m_logoPixmap;
}

void AppInfo::setIconPixmap(QPixmap *p) {
    m_iconPixmap = p;
}

QPixmap * AppInfo::getIconPixmap() {
    if (m_iconPixmap == 0){
        qDebug() << "Created blank pixmap";
        return new QPixmap();
    }
    else
        return m_iconPixmap;
}

void AppInfo::gotReply(QNetworkReply *reply) {

    qDebug() << "Got Pixmap reply; reply: " << reply;

    if (reply->error() == QNetworkReply::NoError)
    {
        if (reply == m_iconReply) {
            m_iconPixmap = new QPixmap();
            m_iconPixmap->loadFromData(reply->readAll());
            m_iconReply = 0;
        } else {
            m_logoPixmap = new QPixmap();
            m_logoPixmap->loadFromData(reply->readAll());
            m_logoReply = 0;
        }

    } else {
        qDebug() << reply->errorString();
        qDebug() << reply->request().url().toString();
    }


    reply->deleteLater();

}



}
}
