#ifndef WEBVIEW_H
#define WEBVIEW_H
#include <QtWebKit>
#include <QWebView>

class WebView : public QWebView
{
Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);
    QString getSessionKey();
    QString getUID();
    QString getSecret();
    bool permissionsGranted();

signals:
    void authReceived();
    void authFailed();

public slots:
    void loadFinished();

private:
    QString m_sessionKey;
    QString m_uid;
    QString m_secret;
    bool m_permissionsGranted;
};

#endif // WEBVIEW_H
