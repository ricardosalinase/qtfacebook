#include "notifications_widget.h"
#include <QGridLayout>
#include <QDebug>

namespace GUI {
namespace Notifications {

Widget::Widget(QString text, QPixmap *p, QWidget *parent) :
    QWidget(parent)
{
    m_label = new QLabel(text);
    m_icon = new QLabel();
    m_icon->setPixmap(*p);
    m_label->setWordWrap(true);



    QGridLayout *gl = new QGridLayout();
    gl->addWidget(m_icon,0,0,Qt::AlignTop);

    gl->addWidget(m_label,0,1,Qt::AlignTop);
    gl->setColumnStretch(1,1);

    setLayout(gl);

    connect(m_label, SIGNAL(linkActivated(QString)),
            this, SLOT(linkClicked(QString)));

}

Widget::~Widget() {

}

void Widget::linkClicked(QString url) {
    qDebug() << url;
}


} // namespace Notifications
} // namespace GUI
