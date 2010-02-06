#ifndef NOTIFICATIONCENTERLABEL_H
#define NOTIFICATIONCENTERLABEL_H

#include <QLabel>

namespace GUI {

class NotificationCenterLabel : public QLabel
{
Q_OBJECT
public:
    explicit NotificationCenterLabel(QWidget *parent = 0);

    virtual QString getNotificationId() = 0;
    virtual QString getToolTipText() = 0;

signals:

public slots:


};

}
#endif // NOTIFICATIONCENTERLABEL_H
