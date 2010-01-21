#ifndef FBCONNECTWIZARD_H
#define FBCONNECTWIZARD_H

#include <QWizard>
#include <QLabel>
#include "userinfo.h"

class WebView;

class FBConnectWizard : public QWizard
{
    Q_OBJECT


public:
    FBConnectWizard(QString apiKey, QString appName, bool firstTime = true);
    enum { Page_Intro, Page_Connect, Page_Conclusion };

public slots:

signals:
    void userHasAuthenticated(UserInfo *info);

private:

    QWizardPage* createIntroPage();
    QWizardPage* createConnectPage();
    QWizardPage* createConclusionPage();



    QString m_apiKey;
    QString m_appName;
    bool m_firstTime;
    WebView *m_view;


};

class ConnectPage : public QWizardPage
{
    Q_OBJECT

public:
    ConnectPage(QString apKey, QWidget *parent = 0);
    void initializePage();
    bool isComplete() const;
    bool hasCompletedAuth();

signals:
    void userAuthenticated(UserInfo *info);

public slots:
    void gotAuth();
    void gotFailed();

private:
    WebView *m_view;
    QString m_apiKey;
    bool m_isComplete;
    bool m_gotAuth;
    QString m_facebookUrl;
    QLabel *m_thumbsUp;
    QLabel *m_thumbsDown;
};

#endif // FBCONNECTWIZARD_H
