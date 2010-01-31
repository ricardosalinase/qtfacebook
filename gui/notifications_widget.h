#ifndef NOTIFICATIONS_WIDGET_H
#define NOTIFICATIONS_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

namespace GUI {
namespace Notifications {

class Widget : public QWidget
{
Q_OBJECT
public:
    Widget(QString text, QPixmap *p, QWidget *parent = 0);
    ~Widget();


signals:
    void notificationLinkClicked(QString url);

public slots:
    void linkClicked(QString url);

private:
    QLabel *m_icon;
    QLabel *m_label;
};

} // namespace Notifications
} // namespace GUI

#endif // NOTIFICATIONS_WIDGET_H
