#ifndef FACEBOOKLOGINDIALOG_H
#define FACEBOOKLOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "userinfo.h"


namespace GUI {

class FacebookLoginDialog : public QDialog
{
Q_OBJECT
public:
    explicit FacebookLoginDialog(UserInfo *userInfo, QWidget *parent = 0);

signals:
    void loginEntered();
    void loginCanceled();

public slots:

private slots:
    void enableLoginButton();
    void loginButtonClicked();

private:
    UserInfo *m_userInfo;
    QLineEdit *m_emailEdit;
    QLineEdit *m_passEdit;
    QPushButton *m_cancelButton;
    QPushButton *m_loginButton;

};

} // namespace GUI

#endif // FACEBOOKLOGINDIALOG_H
