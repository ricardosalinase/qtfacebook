#ifndef BUDDYLISTWIDGET_H
#define BUDDYLISTWIDGET_H

#include <QWidget>
#include <QScrollArea>


#include "FbChatConnection.h"
#include "gui/BuddyWidget.h"

namespace GUI {

class BuddyListWidget : public QWidget
{
Q_OBJECT
public:
    explicit BuddyListWidget(QWidget *parent = 0);

signals:

public slots:
    void gotFbPresence(QXmppPresence);


private:
    QScrollArea *m_scrollArea;
    QWidget *m_bContainer;
    FbChatConnection *m_fbConnection;
    QMap<QString, QString> m_photoMap;
    QMap<QString, GUI::BuddyWidget * > m_buddyMap;
    QMap<QString, GUI::BuddyWidget * > m_waitingForVCardMap;
};

} // namespace GUI

#endif // BUDDYLISTWIDGET_H
