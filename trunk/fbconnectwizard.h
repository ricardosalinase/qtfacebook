#ifndef FBCONNECTWIZARD_H
#define FBCONNECTWIZARD_H

#include <QWizard>


class WebView;

class FBConnectWizard : public QWizard
{
    Q_OBJECT


public:
    FBConnectWizard(QString appName, bool firstTime = true, QWidget *parent = 0);
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


};

class ConnectPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConnectPage(QWidget *parent = 0);
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
