#ifndef BUDDYWIDGET_H
#define BUDDYWIDGET_H

#include <QWidget>

#include "data/FbBuddy.h"

namespace GUI {

class BuddyWidget : public QWidget
{
Q_OBJECT
public:
    explicit BuddyWidget(DATA::FbBuddy *buddy, QWidget *parent = 0);

    DATA::FbBuddy * getFbBuddy();

signals:

public slots:

private:
    DATA::FbBuddy *m_buddy;

};

} // namespace GUI

#endif // BUDDYWIDGET_H
