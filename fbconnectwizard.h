#ifndef FBCONNECTWIZARD_H
#define FBCONNECTWIZARD_H

#include <QWizard>
#include <QNetworkReply>
#include <QLineEdit>
#include <QLabel>
#include "userinfo.h"
#include "util/cookiejar.h"

class WebView;

class FBConnectWizard : public QWizard
{
    Q_OBJECT


public:
    FBConnectWizard(UserInfo *userInfo, QString appName, bool firstTime = true);
    enum { Page_Intro, Page_Connect, Page_Conclusion, Page_Error };


public slots:

signals:
    void userHasAuthenticated();

private:

    QWizardPage* createIntroPage();
    QWizardPage* createConnectPage();
    QWizardPage* createConclusionPage();
    QWizardPage* createErrorPage();



    QString m_appName;
    bool m_firstTime;
    WebView *m_view;
    UserInfo *m_userInfo;


};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(UserInfo *userInfo, QString appName, QWidget *parent = 0);
    bool isComplete() const;
    // Overrides parent method
    int nextId();


signals:


public slots:
    void loginClicked();
    void gotLoginResults(bool success);


private:
    bool m_gotLoggedIn;
    QLineEdit *m_emailEdit;
    QLineEdit *m_pwEdit;
    QPushButton *m_login;
    QNetworkAccessManager *m_nam;
    UserInfo *m_userInfo;
    QLabel *m_loginResult;

};

class ConnectPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConnectPage(UserInfo *userInfo, QWidget *parent = 0);

    void initializePage();
    bool isComplete() const;
    bool hasCompletedAuth();
    // Overrides parent method
    int nextId() const;

signals:
    void userAuthenticated();

public slots:
    void gotAuth();
    void gotFailed();

private:
    WebView *m_view;
    bool m_isComplete;
    bool m_gotAuth;
    QString m_facebookUrl;
    UserInfo *m_userInfo;

};

class ErrorPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ErrorPage(QWidget *parent = 0);
    bool isComplete() const;


};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConclusionPage(QWidget *parent = 0);
    int nextId() const;
};

#endif // FBCONNECTWIZARD_H
