#ifndef APPINFO_H
#define APPINFO_H

#include <QLabel>

namespace DATA {
/*!
 * The AppInfo class encapsulates information about the Facebook applications used to post comments.
 * The Facebook API method "notifications.getList" returns one <app_info> section for every application
 * used to post a comment that it also returned. This includes the standard Facebook UI itself.
 * (See: Notification )
 */
class AppInfo : public QLabel
{
Q_OBJECT
public:
    explicit AppInfo(QWidget *parent = 0);
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
    /*!
     * Store a QPixmap, usually created from the <icon_url> (See: getIconUrl()).
     * This pixmap is current displayed when this object is shown.
     * Note that QPixmap is copied - the poiner is not stored, the same way a QLabel manages pixmaps.
     */
    void setIconPixmap(QPixmap *p);
    /*!
     * Returns a copy of the QPixmap, usually created from the <icon_url> and stored with setIconPixmap(). (See: getIconUrl())
     */
    QPixmap * getIconPixmap();
    void setLogoUrl(QString lu);
    /*!
     * Returns the <logo_url> for this Facebook application. A larger icon set by the Application developer.
     */
    QString getLogoUrl();
    /*!
     * Store a QPixmap, usually created from the <logo_url> (See: getLogoUrl()).
     * Note that the QPixmap is copied - the poiner is not stored, the same way a QLabel manages pixmaps.
     */
    void setLogoPixmap(QPixmap *p);
    /*!
     * Returns a copy of the QPixmap, usually created from the <icon_url> and stored with setLogoPixmap().(See: getIconUrl())
     */
    QPixmap * getLogoPixmap();
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
    bool displayLogoPixmap;
    QPixmap *m_iconPixmap;
    QPixmap *m_logoPixmap;
signals:

public slots:

};

} // namespace DATA

#endif // APPINFO_H
