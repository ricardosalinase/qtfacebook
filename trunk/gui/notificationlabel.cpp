#include "notificationlabel.h"

namespace GUI {

NotificationLabel::NotificationLabel(DATA::Notification *n, QWidget *parent) :
        QLabel(parent)
{
    setText(n->getTitleHtml());
}



} // namespace GUI
