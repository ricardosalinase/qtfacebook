#include "BuddyWidget.h"

#include <QLabel>
#include <QHBoxLayout>

namespace GUI {

BuddyWidget::BuddyWidget(DATA::FbBuddy *, QWidget *parent) :
    QWidget(parent),
    m_buddy(buddy)
{

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QLabel *l = new QLabel(buddy->getFullName());

    mainLayout->addWidget(l);

    setLayout(mainLayout);

}

DATA::FbBuddy * BuddyWidget::getFbBuddy()
{
    return m_buddy;
}

} // namespace GUI
