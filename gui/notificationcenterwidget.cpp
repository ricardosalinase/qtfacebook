#include "notificationcenterwidget.h"
#include <QGridLayout>
#include <QDebug>
#include <QPainter>

namespace GUI {

NotificationCenterWidget::NotificationCenterWidget(GUI::NotificationCenterLabel *n, GUI::AppInfoLabel *a, QWidget *parent) :
        QWidget(parent),
        isStopping(false)
{
    n->setParent(this);
    m_label = n;
    a->setParent(this);
    m_icon = a;
    setBackgroundRole(QPalette::Window);

    QGridLayout *gl = new QGridLayout();
    gl->setHorizontalSpacing(10);
    gl->addWidget(m_icon,0,0,Qt::AlignTop | Qt::AlignLeft);

    gl->addWidget(m_label,0,1,Qt::AlignTop);
    gl->setColumnStretch(1,1);

    setLayout(gl);

    connect(m_label, SIGNAL(linkActivated(QString)),
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

    qDebug() << "changeEvent()" << event->type();

    // stop the pulsing widgets
    if (event->type() == QEvent::Enter) {
        if (timeLine->state() == QTimeLine::Running && !isStopping) {
            stopAfter(5);
            emit acknowledged(m_label->getNotificationId());
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


} // namespace GUI
