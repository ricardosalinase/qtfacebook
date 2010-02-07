#include "facebooklogindialog.h"
#include <QFormLayout>
#include <QHBoxLayout>

namespace GUI {

FacebookLoginDialog::FacebookLoginDialog(UserInfo *userInfo, QWidget *parent) :
    QDialog(parent),
    m_userInfo(userInfo)
{

    resize(400,160);

    m_emailEdit = new QLineEdit();
    m_emailEdit->setText(m_userInfo->getEmailAddy());


    m_passEdit = new QLineEdit();
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_cancelButton = new QPushButton(tr("Cancel"));
    m_loginButton = new QPushButton(tr("Login"));

    m_loginButton->setEnabled(false);
    m_loginButton->setDefault(true);

    connect(m_passEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(m_loginButton, SIGNAL(clicked()),
            this, SLOT(loginButtonClicked()));
    connect(m_cancelButton, SIGNAL(clicked()),
            this, SIGNAL(loginCanceled()));

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Email:"), m_emailEdit);
    layout->addRow(tr("Password:"), m_passEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_loginButton);
    layout->addRow(buttonLayout);

    setLayout(layout);


}

void FacebookLoginDialog::enableLoginButton() {
    m_loginButton->setEnabled(true);
}

void FacebookLoginDialog::loginButtonClicked() {
    m_userInfo->setEmailAddy(m_emailEdit->text());
    m_userInfo->setPass(m_passEdit->text());
    emit loginEntered();

}



} // namespace GUI
