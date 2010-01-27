#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>

namespace API {
namespace Notifications {

class Notification
{
public:
    Notification();
    ~Notification() {};

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

} // End namespace Notifications
} // End namespace API
#endif // NOTIFICATION_H
