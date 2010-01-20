#ifndef FBCONNECTWIZARD_H
#define FBCONNECTWIZARD_H

#include <QWizard>

class WebView;

class FBConnectWizard : public QWizard
{
    Q_OBJECT


public:
    FBConnectWizard(QString apiKey, QString appName, bool firstTime = true);
    enum { Page_Intro, Page_Connect, Page_Conclusion };

public slots:
    void gotAuth();
    void gotFailed();

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


public slots:
    void gotAuth();
    void gotFailed();

private:
    WebView *m_view;
    QString m_apiKey;
    bool m_isComplete;
    bool m_gotAuth;
    QString m_facebookUrl;
};

#endif // FBCONNECTWIZARD_H
