#include "notificationwidget.h"
#include <QGridLayout>
#include <QDebug>
#include <QPainter>

namespace GUI {

NotificationWidget::NotificationWidget(GUI::NotificationLabel *n, GUI::AppInfoLabel *a, QWidget *parent) :
        QWidget(parent),
        isStopping(false)
{
    n->setParent(this);
    m_label = n;
    a->setParent(this);
    m_icon = a;
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
    timeLine->setLoopCount(0);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(update()));
    //connect(timeLine,SIGNAL(finished()), this, SLOT(reverseTimeline()));
}

NotificationWidget::NotificationWidget(QString text, QPixmap *p, QWidget *parent) :
    QWidget(parent)
{
    m_label = new QLabel("<style type=\"text/css\">a { text-decoration: none; }</style>" + text );
    m_label->setStyleSheet("QLabel { font-size : 12px; text-decoration : none;  }");
    m_icon = new QLabel();
    m_icon->setPixmap(*p);
    m_label->setWordWrap(true);


    QGridLayout *gl = new QGridLayout();
    gl->setHorizontalSpacing(10);
    gl->addWidget(m_icon,0,0,Qt::AlignTop | Qt::AlignLeft);

    gl->addWidget(m_label,0,1,Qt::AlignTop);
    gl->setColumnStretch(1,1);

    setLayout(gl);

    // Pass-through
    connect(m_label, SIGNAL(linkActivated(QString)),
            this, SIGNAL(linkActivated(QString)));

    timeLine = new QTimeLine(2000, this);
    timeLine->setFrameRange(2000, 0);
    timeLine->setLoopCount(0);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(update()));
    //connect(timeLine, SIGNAL(finished()),
      //      this, SLOT(reverseTimeline()));
}

NotificationWidget::~NotificationWidget() {
    //delete m_label;
    //delete m_icon;
}

void NotificationWidget::start()
{
   timeLine->start();

}

void NotificationWidget::stopAfter(int lc) {
    if (timeLine->state() == QTimeLine::Running && !isStopping) {
        timeLine->setLoopCount(lc);
        isStopping = true;
    }
}



void NotificationWidget::paintEvent(QPaintEvent *) {



    QPainter painter(this);
    qreal frame = timeLine->currentFrame();
    if (frame <= 1000)
        painter.setOpacity(frame / 1000.);
    else
        painter.setOpacity( (1000 - (frame - 1000)) / 1000);

    //painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter.setWindow(QRect(0, 0, 10, 10));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::blue);
    painter.drawRect(0, 0, 10, 10);



}


} // namespace GUI
