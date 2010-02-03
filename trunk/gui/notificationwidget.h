#ifndef NOTIFICATIONSWIDGET_H
#define NOTIFICATIONSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

#include "gui/appinfolabel.h"
#include "gui/notificationlabel.h"

namespace GUI {


class NotificationWidget : public QWidget
{
Q_OBJECT
public:
    NotificationWidget(QString text, QPixmap *p, QWidget *parent = 0);
    NotificationWidget(GUI::NotificationLabel *n, GUI::AppInfoLabel *a, QWidget *parent = 0);
    ~NotificationWidget();


signals:
    void linkActivated(QString url);

private:
    QLabel *m_icon;
    QLabel *m_label;
};


} // namespace GUI

#endif // NOTIFICATIONSWIDGET_H
