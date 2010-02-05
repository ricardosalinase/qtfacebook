#include "notificationlabel.h"
#include <QPalette>


namespace GUI {

NotificationLabel::NotificationLabel(DATA::Notification *n, QWidget *parent) :
        QLabel(parent),
        m_notification(n)
{
    setText("<style type=\"text/css\">a { text-decoration: none; }</style>" + n->getTitleHtml());
    setStyleSheet("QLabel { font-size : 12px; text-decoration : none;  }");
    setWordWrap(true);

}

NotificationLabel::~NotificationLabel() {
    delete m_notification;
}

DATA::Notification * NotificationLabel::getNotification() {
    return m_notification;
}



} // namespace GUI
