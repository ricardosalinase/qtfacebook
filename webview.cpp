#include "webview.h"
#include <QtDebug>
#include <QRegExp>

WebView::WebView(QWidget *parent) :
    QWebView(parent),
    m_readPermission(false),
    m_publishPermission(false),
    m_offlineAccessPermission(false),
    m_readMailbox(false),
    m_xmppLogin(false)
{
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinished()));


}

void WebView::loadFinished() {

    //qDebug() << page()->currentFrame()->toHtml();



    if (page()->findText("Success"))
    {
        QString str = url().toString();
        qDebug() << str;

        // Another facebook bug. They return two different versions of the JSON object depending on state
        QRegExp rx("session_key\":\"([^\"]+)\",\"uid\":(\\d+),\"expires\":(\\d+),\"secret\":\"([^\"]+)\",");

        if (rx.indexIn(str) != -1) {
            //qDebug() << "Session Key: " << rx.cap(1) << "\nuid: " << rx.cap(2) << "\nExpires: " << rx.cap(3) << "\nSecret: " << rx.cap(4);

            m_sessionKey = rx.cap(1);
            m_uid = rx.cap(2);
            m_secret = rx.cap(4);

        } else {

            rx.setPattern("session_key\":\"([^\"]+)\",\"uid\":\"(\\d+)\",\"expires\":(\\d+),\"secret\":\"([^\"]+)\",");

            if (rx.indexIn(str) != -1) {
                //qDebug() << "Session Key: " << rx.cap(1) << "\nuid: " << rx.cap(2) << "\nExpires: " << rx.cap(3) << "\nSecret: " << rx.cap(4);

                m_sessionKey = rx.cap(1);
                m_uid = rx.cap(2);
                m_secret = rx.cap(4);
            }
        }

        //rx.setPattern("\\[\"read_stream\",\"offline_access\",\"publish_stream\",\"read_mailbox\"\\]");
        rx.setPattern("permissions=\\[([^\\]]+)\\]");
        if (rx.indexIn(str) != -1) {

            QString perms = rx.cap(1);

            rx.setPattern("read_stream");
            if (rx.indexIn(perms) != -1)
                m_readPermission = true;

            rx.setPattern("offline_access");
            if (rx.indexIn(perms) != -1)
                m_offlineAccessPermission = true;

            rx.setPattern("publish_stream");
            if (rx.indexIn(perms) != -1)
                m_publishPermission = true;

            rx.setPattern("read_mailbox");
            if (rx.indexIn(perms) != -1)
                m_readMailbox = true;

            rx.setPattern("xmpp_login");
            if (rx.indexIn(perms) != -1)
                m_xmppLogin = true;
       }

        emit authReceived();


    } else if (page()->findText("Failure")) {
        // Whoops. Something went wrong
        emit authFailed();
    }


}

QString WebView::getSessionKey() {
    return m_sessionKey;
}

QString WebView::getSecret() {
    return m_secret;
}

QString WebView::getUID() {
    return m_uid;
}

bool WebView::hasReadPermission()
{
    return m_readPermission;
}

bool WebView::hasPublishPermission()
{
    return m_publishPermission;
}

bool WebView::hasOfflineAccessPermission()
{
    return m_offlineAccessPermission;
}

bool WebView::permissionsGranted() {
    return(m_readPermission &&
           m_offlineAccessPermission &&
           m_publishPermission &&
           m_readMailbox &&
           m_xmppLogin);
}
