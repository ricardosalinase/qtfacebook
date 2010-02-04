#ifndef NOTIFICATIONSWIDGET_H
#define NOTIFICATIONSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimeLine>
#include <QPaintEvent>

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

    void start();
    void stopAfter(int loopCount);

signals:
    void linkActivated(QString url);

protected:
    void paintEvent(QPaintEvent *event);

private slots:

private:
    QLabel *m_icon;
    QLabel *m_label;
    QTimeLine *timeLine;
    bool isStopping;
};


} // namespace GUI

#endif // NOTIFICATIONSWIDGET_H
