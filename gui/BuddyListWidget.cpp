#include "BuddyListWidget.h"

#include <QVBoxLayout>

#include "util/workerthread.h"
#include "FbChatConnection.h"



namespace GUI {

BuddyListWidget::BuddyListWidget(QWidget *parent) :
    QWidget(parent)
{

    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background,QColor(82,110,166));
    setPalette(palette);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);

    m_bContainer = new QWidget();
    m_bContainer->resize(200,600);
    palette = m_bContainer->palette();
    palette.setColor(QPalette::Background,Qt::white);
    m_bContainer->setPalette(palette);

    QVBoxLayout *vbl = new QVBoxLayout();
    m_bContainer->setLayout(vbl);
    m_scrollArea->setWidget(m_bContainer);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);

    m_fbConnection = new FbChatConnection();

    connect(m_fbConnection, SIGNAL(presenceReceived(QXmppPresence)),
            this, SLOT(gotFbPresence(QXmppPresence)));

    UTIL::WorkerThread *t = new UTIL::WorkerThread(m_fbConnection);
    t->start();

}

void BuddyListWidget::gotFbPresence(QXmppPresence presence)
{
    // first see if we have this user on our list
    if (!m_buddyMap.contains(presence.getFrom()))
    {
        DATA::FbBuddy *fbb = new DATA::FbBuddy();
        BuddyWidget *bw = new BuddyWidget(fbb);

        // Request the vCard
        m_waitingForVCardMap.insert(presence.getFrom(), bw);
    }


    // update widget


}


} // namespace GUI
