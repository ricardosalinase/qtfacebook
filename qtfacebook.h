#ifndef QTFACEBOOK_H
#define QTFACEBOOK_H

#include <QWidget>

#include "userinfo.h"

class QtFacebook : public QWidget
{
Q_OBJECT
public:
    explicit QtFacebook(QWidget *parent = 0);

signals:

public slots:
    void FbWizardComplete(UserInfo *info);

private:
    UserInfo *m_userInfo;

};

#endif // QTFACEBOOK_H
