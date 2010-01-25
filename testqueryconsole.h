#ifndef TESTQUERYCONSOLE_H
#define TESTQUERYCONSOLE_H

#include <QWidget>
#include <QString>
#include <QNetworkReply>

#include "userinfo.h"
#include "api/observer.h"

namespace Ui {
    class TestQueryConsole;
}

class TestQueryConsole : public API::ObserverWidget {
    Q_OBJECT
public:
    TestQueryConsole(UserInfo *userInfo, QWidget *parent = 0);
    ~TestQueryConsole();



protected:
    void changeEvent(QEvent *e);

private:
    Ui::TestQueryConsole *ui;
    UserInfo *m_userInfo;
    QString m_apiKey;
    QNetworkAccessManager *m_manager;

public slots:
    void processMethodResults(QNetworkReply *reply);

private slots:
    void addPostArgs();
    void sendQuery();
    void gotReply(QNetworkReply *reply);


};

#endif // TESTQUERYCONSOLE_H
