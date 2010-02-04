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
    NotificationWidget(GUI::NotificationLabel *n, GUI::AppInfoLabel *a, QWidget *parent = 0);
    ~NotificationWidget();

    void start();
    void stopAfter(int loopCount);

signals:
    void linkActivated(QString url);
    void acknowledged(QString notificationId);

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);

private slots:

private:
    AppInfoLabel *m_icon;
    NotificationLabel *m_label;
    QTimeLine *timeLine;
    bool isStopping;
};


} // namespace GUI

#endif // NOTIFICATIONSWIDGET_H
