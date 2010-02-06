#ifndef NOTIFICATIONLABEL_H
#define NOTIFICATIONLABEL_H

#include <QLabel>
#include <QTimer>

#include "data/notification.h"
#include "notificationcenterlabel.h"

namespace GUI {

class NotificationLabel : public NotificationCenterLabel
{
Q_OBJECT
public:
    NotificationLabel(DATA::Notification *n, QWidget *parent = 0);
    DATA::Notification * getNotification();
    ~NotificationLabel();

    virtual QString getNotificationId();
    virtual QString getToolTipText();

private slots:
    void createDisplayText();

private:
    DATA::Notification *m_notification;

    QTimer *m_timer;
};

} // namespace GUI

#endif // NOTIFICATIONLABEL_H
