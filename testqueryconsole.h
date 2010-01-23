#ifndef TESTQUERYCONSOLE_H
#define TESTQUERYCONSOLE_H

#include <QWidget>
#include <QString>

#include "userinfo.h"

namespace Ui {
    class TestQueryConsole;
}

class TestQueryConsole : public QWidget {
    Q_OBJECT
public:
    TestQueryConsole(QString apiKey, UserInfo *userInfo, QWidget *parent = 0);
    ~TestQueryConsole();



protected:
    void changeEvent(QEvent *e);

private:
    Ui::TestQueryConsole *ui;
    UserInfo *m_userInfo;
    QString m_apiKey;


private slots:
    void addPostArgs();
    void sendQuery();


};

#endif // TESTQUERYCONSOLE_H
