#ifndef QTFACEBOOK_H
#define QTFACEBOOK_H

#include <QWidget>
#include <QVBoxLayout>

#include "userinfo.h"
#include "fbconnectwizard.h"

class QtFacebook : public QWidget
{
Q_OBJECT
public:
    explicit QtFacebook(QWidget *parent = 0);

signals:

public slots:
    void fbWizardComplete();
    void saveUserInfo(UserInfo *info);
    void fbWizardCanceled();

private:
    UserInfo *m_userInfo;
    bool loadUserInfo();
    QVBoxLayout *m_layout;
    FBConnectWizard *m_wizard;
};

#endif // QTFACEBOOK_H
