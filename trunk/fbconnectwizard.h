#ifndef FBCONNECTWIZARD_H
#define FBCONNECTWIZARD_H

#include <QWizard>

class WebView;

class FBConnectWizard : public QWizard
{
    Q_OBJECT

public:
    explicit FBConnectWizard(QString apiKey, QString appName, bool firstTime);

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

#endif // FBCONNECTWIZARD_H
