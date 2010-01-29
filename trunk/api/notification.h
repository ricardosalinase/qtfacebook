#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>

namespace API {
namespace Notifications {

/*!
 * The Notification class encapsulates individual notificaitons returned by the Facebook API call "notifications.getList"
 */
class Notification
{

public:
    /*!
     * Constructs an empty Notification object
     */
    Notification();
    virtual ~Notification() {};

    void setNotificationId(QString nid);
    /*!
     * Returns the <notification_id> returned by Facebook for this notification
     */
    QString getNotificationId();

    void setSenderId(QString sid);
    /*!
     * Returns the <sender_id>) returned by Facebook for this notification. This is the Facebook
     * uid of the author.
     */
    QString getSenderId();
    void setRecipientId(QString rid);
    /*!
     * Returns the <recipient_id>) returned by Facebook for this notification. This is the Facebook
     * uid for the person who originally posted the item being commented on.
     */
    QString getRecipientId();
    void setCreatedTime(QString ctime);
    /*!
     * Returns the <created_time> returned by Facebook for this notification. This is in Unix
     * epoch time.
     */
    QString getCreatedTime();
    void setUpdatedTime(QString utime);
    /*!
     * Returns the <updated_time> returned by Facebook for this notification. This is in Unix
     * epoch time.
     */
    QString getUpdatedTime();
    void setTitleHtml(QString html);
    /*!
     * Returns the <title_html> returned by Facebook for this notification. This is the message
     * seen in the normal Facebook notification widget; e.g "Person_A commented on Person_B's Item_C". It
     * also contains hyperlinks for Person_A, Person_B, and Item_C
     */
    QString getTitleHtml();
    void setTitleText(QString text);
    /*!
     * Returns the <title_text> returned by Facebook for this notification. This is the same content
     * as returned by getTitleHtml() but in plain text.
     */
    QString getTitleText();
    void setBodyHtml(QString html);
    /*!
     * Returns the <body_html> returned by Facebook for this notification. This is the actual comment
     * posted, which is not seen in the standard Facebook notification UI. It is formatted in HTML and
     * may contain hyperlinks
     */
    QString getBodyHtml();
    void setBodyText(QString text);
    /*!
     * Returns the <body_text> returned by Facebook for this notification. This is the actual comment
     * posted, which is not seen in the standard Facebook notification UI. It is the same content as
     * returned by getBodyHtml() but in plain text
     */
    QString getBodyText();
    void setHref(QString href);
    /*!
     * Returns the <href> returned by Facebook for this notification. This is the URL for the item
     * being commented on.
     */
    QString getHref();
    void setAppId(QString aid);
    /*!
     * Returns the <app_id> returned by Facebook for this notification. This is the Id number of the application
     * used to post the comment. (see: AppInfo )
     */
    QString getAppId();
    void setIsRead(bool ir);
    /*!
     * Returns the <is_read> returned by Facebook for this notification. This indicates if the user has already read
     * this notification. The standard Facebook UI changes this to 'true' whenever a user clicks on the notification
     * widget.
     */
    bool getIsRead();
    void setIsHidden(bool ih);
    /*! Returns the <is_hidden> returned by Facebook for this notification. This indicates the user has selected to not
     * see notifications from this person or application.
     */
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

/*!
 * The AppInfo class encapsulates information about the Facebook applications used to post comments.
 * The Facebook API method "notifications.getList" returns one <app_info> section for every application
 * used to post a comment that it also returned. This includes the standard Facebook UI itself.
 * (See: Notification )
 */
class AppInfo
{
public:
    AppInfo();
    ~AppInfo() {};

    void setAppId(QString appId);
    /*!
     * Returns the <api_id> for this Facebook application. This can be cross-referenced to a Notification
     */
    QString getAppId();
    void setApiKey(QString apiKey);
    /*!
     * Returns the <api_key> for this Facebook application.
     */
    QString getApiKey();
    void setDisplayName(QString dn);
    /*!
     * Returns the <display_name> for this Facebook application.
     */
    QString getDisplayName();
    void setIconUrl(QString iu);
    /*!
     * Returns the <icon_url> for this Facebook application. These are the small icons to the left of each notification
     * in the standard Facebook UI and are set by the application ower (including the Facebook UI itself). It may be empty.
     */
    QString getIconUrl();
    void setLogoUrl(QString lu);
    /*!
     * Returns the <logo_url> for this Facebook application. A larger icon set by the Application developer.
     */
    QString getLogoUrl();
    void setCompanyName(QString cn);
    /*!
     * Returns the <company_name> for this Facebook application. It may be empty.
     */
    QString getCompanyName();
    void setDescription(QString d);
    /*!
     * Returns the <description> for this Facebook application. It may be empty.
     */
    QString getDescription();
    void setDailyActiveUsers(QString dau);
    /*!
     * Returns the <daily_active_users> for this Facebook application.
     */
    QString getDailyActiveUsers();
    void setWeeklyActiveUsers(QString wau);
    /*!
     * Returns the <weekly_active_users> for this Facebook application.
     */
    QString getWeeklyActiveUsers();
    void setMonthlyActiveUsers(QString mau);
    /*!
     * Returns the <monthly_active_users> for this Facebook application.
     */
    QString getMonthlyActiveUsers();
    void setCategory(QString c);
    /*!
     * Returns the <category> for this Facebook application. e.g. "Games"
     */
    QString getCategory();
    void setSubCategory(QString sc);
    /*!
     * Returns the <subcategory> for this Facebook application. e.g. "Puzzle Games"
     */
    QString getSubCategory();
    void setCanvasName(QString cn);
    /*!
     * Returns the <canvas_name> for this Facebook application.
     */
    QString getCanvasName();

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
    QString m_canvasName;

};


} // End namespace Notifications
} // End namespace API
#endif // NOTIFICATION_H
