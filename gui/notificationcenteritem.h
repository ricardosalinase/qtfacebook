#ifndef NOTIFICATIONCENTERITEM_H
#define NOTIFICATIONCENTERITEM_H

#include <QString>
#include <QUrl>
#include <QPixmap>

namespace GUI {

/*!
 * (Abstract) Interface used for anything that wants to be displayed in the
 * GUI::NotificationCenter display
 */
class NotificationCenterItem
{
public:
    enum ItemType { Notification, StreamPost };
    virtual QString getNavigationHtml() = 0;
    virtual QString getCreatedTime() = 0;
    virtual QString getNotificationCenterId() = 0;
    virtual ItemType getNotificationCenterItemType() = 0;

protected:
    NotificationCenterItem() {};
};

} // namespace GUI

#endif // NOTIFICATIONCENTERITEM_H
