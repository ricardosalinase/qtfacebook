#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>

namespace API {
namespace Notifications {

class Notification
{

public:
    Notification();
    virtual ~Notification() {};

    void setNotificationId(QString nid);
    QString getNotificaitonId();
    void setSenderId(QString sid);
    QString getSenderId();
    void setRecipientId(QString rid);
    QString getRecipientId();
    void setCreatedTime(QString ctime);
    QString getCreatedTime();
    void setUpdatedTime(QString utime);
    QString getUpdatedTime();
    void setTitleHtml(QString html);
    QString getTitleHtml();
    void setTitleText(QString text);
    QString getTitleText();
    void setBodyHtml(QString html);
    QString getBodyHtml();
    void setBodyText(QString text);
    QString getBodyText();
    void setHref(QString href);
    QString getHref();
    void setAppId(QString aid);
    QString getAppId();
    void setIsRead(bool ir);
    bool getIsRead();
    void setIsHidden(bool ih);
    bool getIsHidden();

private:
    QString m_notificationId;
    QString m_senderId;
    QString m_recipientId;
    QString m_createdTime;
    QString m_updatedTime;
    QString m_titleHtml;
    QString m_titleText;
    QString m_bodyHtml;
    QString m_bodyText;
    QString m_href;
    QString m_appId;
    bool m_isRead;
    bool m_isHidden;

};

class AppInfo
{
public:
    AppInfo();
    ~AppInfo() {};

    void setAppId(QString appId);
    QString getAppId();
    void setApiKey(QString apiKey);
    QString getApiKey();
    void setDisplayName(QString dn);
    QString getDisplayName();
    void setIconUrl(QString iu);
    QString getIconUrl();
    void setLogoUrl(QString lu);
    QString getLogoUrl();
    void setCompanyName(QString cn);
    QString getCompanyName();
    void setDescription(QString d);
    QString getDescription();
    void setDailyActiveUsers(QString dau);
    QString getDailyActiveUsers();
    void setWeeklyActiveUsers(QString wau);
    QString getWeeklyActiveUsers();
    void setMonthlyActiveUsers(QString mau);
    QString getMonthlyActiveUsers();
    void setCategory(QString c);
    QString getCategory();
    void setSubCategory(QString sc);
    QString getSubCategory();

private:
    QString m_appId;
    QString m_apiKey;
    QString m_displayName;
    QString m_iconUrl;
    QString m_logoUrl;
    QString m_companyName;
    QString m_description;
    QString m_dailyActiveUsers;
    QString m_weeklyActiveUsers;
    QString m_monthlyActiveUsers;
    QString m_category;
    QString m_subCategory;

};


} // End namespace Notifications
} // End namespace API
#endif // NOTIFICATION_H
