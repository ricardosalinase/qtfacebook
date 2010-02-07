#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "loginfaileddialog.h"

namespace GUI {

LoginFailedDialog::LoginFailedDialog(QWidget *parent) :
    QDialog(parent)
{
    QLabel *l = new QLabel("Email address or password incorrect.");
    QPushButton *button = new QPushButton("Ok");

    connect(button, SIGNAL(clicked()),
            this, SLOT(accept()));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(l);
    layout->addWidget(button);
    setLayout(layout);

}

} // namespace GUI
