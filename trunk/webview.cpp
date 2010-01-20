#include "webview.h"
#include <QtDebug>
#include <QRegExp>

WebView::WebView(QWidget *parent) :
    QWebView(parent),
    m_permissionsGranted(false)
{
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinished()));


}

void WebView::loadFinished() {

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

            rx.setPattern("\\[\"read_stream\",\"offline_access\",\"publish_stream\"\\]");

            if (rx.indexIn(str))
                m_permissionsGranted = true;

            emit authReceived();

        } else {

            rx.setPattern("session_key\":\"([^\"]+)\",\"uid\":\"(\\d+)\",\"expires\":(\\d+),\"secret\":\"([^\"]+)\",");

            if (rx.indexIn(str) != -1) {
                //qDebug() << "Session Key: " << rx.cap(1) << "\nuid: " << rx.cap(2) << "\nExpires: " << rx.cap(3) << "\nSecret: " << rx.cap(4);

                m_sessionKey = rx.cap(1);
                m_uid = rx.cap(2);
                m_secret = rx.cap(4);

                rx.setPattern("\\[\"read_stream\",\"offline_access\",\"publish_stream\"\\]");

                if (rx.indexIn(str))
                    m_permissionsGranted = true;


                emit authReceived();
            }
        }
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

bool WebView::permissionsGranted() {
    return m_permissionsGranted;
}
