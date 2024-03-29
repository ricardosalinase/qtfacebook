#include <QGridLayout>

#include "notification.h"

namespace DATA {

    Notification::Notification() :
    m_isHidden(false),
    m_isRead(false)
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

void Notification::setAppInfo(AppInfo *info) {
    m_appInfo = *info;
}

AppInfo & Notification::getAppInfo()  {
    return m_appInfo;
}

// NotificationCenterItem Interface
QString Notification::getNavigationHtml() {

    return m_titleHtml;

}

QString Notification::getNotificationCenterId() {
    return m_notificationId;
}


} // namespace Data
