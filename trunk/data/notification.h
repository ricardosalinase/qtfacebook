#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

namespace DATA {
/*!
 * The Notification class encapsulates individual notificaitons returned by the Facebook API call "notifications.getList"
 * It Inherits from QLabel and will display the <title_html> returned from the API call (see: setTitleHtml()).
 */
class Notification : public QLabel
{
Q_OBJECT
public:

    /*!
     * Constructs an empty Notification object
     */
    explicit Notification(QWidget *parent = 0);
    virtual ~Notification() {};
    //Notification(const Notification &orig)();

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

signals:


public slots:



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
    bool m_displayHtml;
    QLabel *m_icon;
    QLabel *m_label;

};

} // namespace Data

#endif // NOTIFICATION_H
