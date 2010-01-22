#ifndef TESTQUERYCONSOLE_H
#define TESTQUERYCONSOLE_H

#include <QWidget>

namespace Ui {
    class TestQueryConsole;
}

class TestQueryConsole : public QWidget {
    Q_OBJECT
public:
    TestQueryConsole(QWidget *parent = 0);
    ~TestQueryConsole();



protected:
    void changeEvent(QEvent *e);

private:
    Ui::TestQueryConsole *ui;

private slots:
    void addPostArgs();
    void sendQuery();


};

#endif // TESTQUERYCONSOLE_H
