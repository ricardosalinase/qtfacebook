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
    bool hasReadPermission();
    bool hasPublishPermission();
    bool hasOfflineAccessPermission();

signals:
    void authReceived();
    void authFailed();

public slots:
    void loadFinished();

private:
    QString m_sessionKey;
    QString m_uid;
    QString m_secret;
    bool m_readPermission;
    bool m_publishPermission;
    bool m_offlineAccessPermission;
};

#endif // WEBVIEW_H
