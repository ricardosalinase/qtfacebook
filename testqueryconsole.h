#ifndef TESTQUERYCONSOLE_H
#define TESTQUERYCONSOLE_H

#include <QWidget>
#include <QString>
#include <QNetworkReply>

#include "util/OurUserInfo.h"
#include "api/apimethod.h"
#include "api/friends_get.h"
#include "api/notifications_getlist.h"

namespace Ui {
    class TestQueryConsole;
}

class TestQueryConsole : public QWidget {
    Q_OBJECT
public:
    explicit TestQueryConsole(QWidget *parent = 0);
    ~TestQueryConsole();



protected:
    void changeEvent(QEvent *e);

private:
    Ui::TestQueryConsole *ui;
    UTIL::OurUserInfo *m_userInfo;
    QString m_apiKey;
    QNetworkAccessManager *m_manager;

public slots:
    void methodReturned(API::Method *method);
    void apiFriendsGet(API::Friends::Get *);
    void apiNotificationsGetList(API::Notifications::GetList*);

private slots:
    void addPostArgs();
    void sendQuery();
    void gotReply(QNetworkReply *reply);


};

#endif // TESTQUERYCONSOLE_H
