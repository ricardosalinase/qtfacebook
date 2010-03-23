#include "notificationcenterwidget.h"

#include <QGridLayout>
#include <QDebug>
#include <QPainter>

#include "util/agestring.h"

namespace GUI {

NCWLabel::NCWLabel(GUI::NotificationCenterItem *i, QWidget *parent) :
        QLabel(parent)
{
    m_createdTime = i->getCreatedTime();
    m_baseHtml = i->getNavigationHtml();

    createDisplayText();
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(createDisplayText()));
    m_timer->start(60000);
}



void NCWLabel::createDisplayText() {

    QString age = UTIL::ageString(m_createdTime);

    setText("<style type=\"text/css\">a { text-decoration: none; }</style>"
            + m_baseHtml
            + "<BR><font style=\"font-size : 8px;\"> " + age);
    setWordWrap(true);

}

NotificationCenterWidget::NotificationCenterWidget(GUI::NotificationCenterItem *i, QLabel *pmLabel, QWidget *parent) :
    QWidget(parent),
    isNew(false),
    m_item(i)
{
    setBackgroundRole(QPalette::Window);

    QGridLayout *gl = new QGridLayout();

    gl->setHorizontalSpacing(10);
    gl->addWidget(pmLabel,0,0,Qt::AlignTop | Qt::AlignLeft);

    NCWLabel *label = new NCWLabel(i,this);
    gl->addWidget(label,0,1,Qt::AlignTop);
    gl->setColumnStretch(1,1);

    setLayout(gl);

    connect(label, SIGNAL(linkActivated(QString)),
            this, SIGNAL(linkActivated(QString)));

}


NotificationCenterWidget::~NotificationCenterWidget() {

}

void NotificationCenterWidget::start()
{
   isNew = true;
   this->setAutoFillBackground(true);
   QPalette palette = this->palette();
   palette.setColor(QPalette::Background,QColor(250,250,255));
   setPalette(palette);
}
void NotificationCenterWidget::mousePressEvent ( QMouseEvent * event )
{
    if (isNew) {
        stop();
        isNew = false;
        emit acknowledged(m_item->getNotificationCenterItemType(),
                          m_item->getNotificationCenterId());
    }
}

void NotificationCenterWidget::enterEvent(QEvent *event) {

//    if (event->type() == QEvent::Enter) {
//        if (isNew) {
//            stop();
//            isNew = false;
//            emit acknowledged(m_item->getNotificationCenterItemType(),
//                              m_item->getNotificationCenterId());
//        }
//    }
}

void NotificationCenterWidget::stop() {

    if (isNew) {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Background,Qt::white);
        setPalette(palette);
    }
}



void NotificationCenterWidget::paintEvent(QPaintEvent *) {

    QPainter painter(this);

    painter.setWindow(QRect(0, 0, 10, 10));
    painter.setRenderHint(QPainter::Antialiasing);


    if (isNew) {
        painter.setPen(Qt::blue);
        painter.drawRect(0, 0, 10, 10);

    } else {
        painter.setPen(Qt::gray);
        painter.drawLine(0,10,10,10);

    }


}

QString NotificationCenterWidget::getNotificationCenterId() {
    return m_item->getNotificationCenterId();
}


} // namespace GUI
