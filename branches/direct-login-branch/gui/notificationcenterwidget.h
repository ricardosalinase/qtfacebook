#ifndef NOTIFICATIONCENTERWIDGET_H
#define NOTIFICATIONCENTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimeLine>
#include <QPaintEvent>

#include "gui/appinfolabel.h"
#include "gui/notificationcenterlabel.h"

namespace GUI {


class NotificationCenterWidget : public QWidget
{
Q_OBJECT
public:
    NotificationCenterWidget(GUI::NotificationCenterLabel *n, GUI::AppInfoLabel *a, QWidget *parent = 0);
    ~NotificationCenterWidget();

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
    NotificationCenterLabel *m_label;
    QTimeLine *timeLine;
    bool isStopping;
};


} // namespace GUI

#endif // NOTIFICATIONCENTERWIDGET_H
