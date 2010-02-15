#include "notificationlabel.h"
#include <QDateTime>
#include "util/agestring.h"


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

    QString age = UTIL::ageString(m_notification->getCreatedTime());

    setText("<style type=\"text/css\">a { text-decoration: none; }</style>"
            + m_notification->getTitleHtml()
            + "<BR><font style=\"font-size : 8px;\"> " + age);

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
