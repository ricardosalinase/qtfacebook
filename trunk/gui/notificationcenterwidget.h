#ifndef NOTIFICATIONCENTERWIDGET_H
#define NOTIFICATIONCENTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimeLine>
#include <QPaintEvent>
#include <QTimer>

#include "gui/notificationcenteritem.h"

namespace GUI {

class NCWLabel : public QLabel
{
    Q_OBJECT
public:
    NCWLabel(GUI::NotificationCenterItem *i, QWidget *parent = 0);

private slots:
    void createDisplayText();

private:
    QString m_baseHtml;
    QString m_createdTime;

    QTimer *m_timer;
};


class NotificationCenterWidget : public QWidget
{
Q_OBJECT
public:
    NotificationCenterWidget(GUI::NotificationCenterItem *i, QLabel *pmLabel, QWidget *parent = 0);
    ~NotificationCenterWidget();
    QString getNotificationCenterId();
    void start();
    void stopAfter(int loopCount);

signals:
    void linkActivated(QString url);
    void acknowledged(NotificationCenterItem::ItemType type, QString id);

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);

private slots:

private:
    QTimeLine *timeLine;
    bool isStopping;
    NotificationCenterItem *m_item;
};


} // namespace GUI

#endif // NOTIFICATIONCENTERWIDGET_H
