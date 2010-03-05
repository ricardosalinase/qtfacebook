#include "LoadingProgressWidget.h"

#include <QHBoxLayout>
#include <QLabel>

namespace GUI {

LoadingProgressWidget::LoadingProgressWidget(const QString& label, int height,
                                             int width, QWidget *parent) :
    QWidget(parent)
{
    resize(width, height);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QHBoxLayout *mainLayout = new QHBoxLayout();

    QLabel *l = new QLabel(label);
    l->setStyleSheet("font-size: 8pt;");
    mainLayout->addWidget(l,0,Qt::AlignCenter);

    m_progressBar = new QProgressBar();
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(0);
    m_progressBar->setMaximumHeight(10);
    mainLayout->addWidget(m_progressBar,0,Qt::AlignCenter);

    setLayout(mainLayout);


}

} // namespace GUI
