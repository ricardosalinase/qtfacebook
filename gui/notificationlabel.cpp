#include "notificationlabel.h"
#include <QDateTime>


namespace GUI {

NotificationLabel::NotificationLabel(DATA::Notification *n, QWidget *parent) :
        NotificationCenterLabel(parent),
        m_notification(n)
{
    createDisplayText();
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(createDisplayText()));
    m_timer->start(60000);

}

NotificationLabel::~NotificationLabel() {
    delete m_notification;
}

DATA::Notification * NotificationLabel::getNotification() {
    return m_notification;
}

void NotificationLabel::createDisplayText() {

    uint createTime = m_notification->getCreatedTime().toUInt();
    uint now = QDateTime::currentDateTime().toUTC().toTime_t();
    uint diff = (now - createTime);

    QString unit;

    if (diff < 60)
        unit = "seconds ago";
    else if (diff < 3600) {
        diff = diff / 60;
        diff > 1 ? unit = "minutes ago" : unit = "minute ago";
    } else if (diff < 86400) {
        diff = diff / 3600;
        diff > 1 ? unit = "hours ago" : unit = "hour ago";
    } else {
        diff = diff / 86400;
        diff > 1 ? unit = "days ago" : unit = "day ago";
    }

    setText("<style type=\"text/css\">a { text-decoration: none; }</style>"
            + m_notification->getTitleHtml()
            + "<BR><font style=\"font-size : 8px;\"> " + QString::number(diff) + " " + unit);

    setStyleSheet("QLabel { font-size : 12px; text-decoration : none;  }");
    setWordWrap(true);

}

QString NotificationLabel::getNotificationId() {
    return m_notification->getNotificationId();
}

QString NotificationLabel::getToolTipText() {
    return m_notification->getBodyText();
}

} // namespace GUI
