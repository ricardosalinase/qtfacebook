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
    isStopping(false),
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

    timeLine = new QTimeLine(2000, this);
    timeLine->setFrameRange(2000, 0);
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    timeLine->setLoopCount(0);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(update()));

}


NotificationCenterWidget::~NotificationCenterWidget() {

}

void NotificationCenterWidget::start()
{
   timeLine->start();

}

void NotificationCenterWidget::enterEvent(QEvent *event) {

    //qDebug() << "changeEvent()" << event->type();

    // stop the pulsing widgets
    if (event->type() == QEvent::Enter) {
        if (timeLine->state() == QTimeLine::Running && !isStopping) {
            stopAfter(5);
            emit acknowledged(m_item->getNotificationCenterItemType(),
                              m_item->getNotificationCenterId());
        }
    }
}

void NotificationCenterWidget::stopAfter(int lc) {
    if (timeLine->state() == QTimeLine::Running && !isStopping) {
        qDebug() << "Stopping...";
        timeLine->stop();

        timeLine->setLoopCount(lc);
        timeLine->setDuration(500);
        timeLine->start();
        isStopping = true;
    }
}



void NotificationCenterWidget::paintEvent(QPaintEvent *) {

    //qDebug() << timeLine->currentFrame();

    QPainter painter(this);

    painter.setWindow(QRect(0, 0, 10, 10));
    painter.setRenderHint(QPainter::Antialiasing);


    if (timeLine->state() == QTimeLine::Running) {
        qreal frame = timeLine->currentFrame();
        if (frame <= 1000)
            painter.setOpacity(frame / 1000.);
        else
            painter.setOpacity( (1000 - (frame - 1000)) / 1000);
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
