#ifndef NOTIFICATIONLABEL_H
#define NOTIFICATIONLABEL_H

#include <QLabel>

#include "data/notification.h"

namespace GUI {

class NotificationLabel : public QLabel
{
Q_OBJECT
public:
    NotificationLabel(DATA::Notification *n, QWidget *parent = 0);
    DATA::Notification * getNotification();
    ~NotificationLabel();
    //void setNotification(DATA::Notification *n);
private:
    DATA::Notification *m_notification;

};

} // namespace GUI

#endif // NOTIFICATIONLABEL_H
