#ifndef NOTIFICATIONS_WIDGET_H
#define NOTIFICATIONS_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMetaType>

#include "gui/appinfolabel.h"
#include "gui/notificationlabel.h"

namespace GUI {
namespace Notifications {

class Widget : public QWidget
{
Q_OBJECT
public:
    Widget(QString text, QPixmap *p, QWidget *parent = 0);
    Widget(GUI::NotificationLabel *n, GUI::AppInfoLabel *a, QWidget *parent = 0);
    ~Widget();


signals:
    void linkActivated(QString url);

private:
    QLabel *m_icon;
    QLabel *m_label;
};

} // namespace Notifications
} // namespace GUI
Q_DECLARE_METATYPE(GUI::Notifications::Widget*)
#endif // NOTIFICATIONSWIDGET_H
