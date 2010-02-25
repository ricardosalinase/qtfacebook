#include "notifications_widget.h"
#include <QGridLayout>
#include <QDebug>

namespace GUI {
namespace Notifications {

    Widget::Widget(GUI::NotificationLabel *n, GUI::AppInfoLabel *a, QWidget *parent) :
        QWidget(parent)
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

}

Widget::Widget(QString text, QPixmap *p, QWidget *parent) :
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


}

Widget::~Widget() {

}




} // namespace Notifications
} // namespace GUI