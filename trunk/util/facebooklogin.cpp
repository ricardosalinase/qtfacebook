#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>

#include "facebooklogin.h"




namespace UTIL {

FacebookLogin::FacebookLogin(UserInfo *userInfo, QObject *parent) :
            QObject(parent),
            m_userInfo(userInfo)
{
}

void FacebookLogin::logIn() {

    m_nam = new QNetworkAccessManager();
    m_nam->setCookieJar(m_userInfo->getCookieJar());
    m_userInfo->getCookieJar()->setParent(0);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
            this,SLOT(gotNetworkReply(QNetworkReply*)));

    QUrl url("https://login.facebook.com/login.php");
    QByteArray args;
    args.append("email=" + m_userInfo->getEmailAddy() +
                "&pass=" + m_userInfo->getPass());

    QByteArray exclude("&=");
    QByteArray include;
    args = args.toPercentEncoding(exclude,include,'%');

    QNetworkRequest nr;
    nr.setUrl(url);
    nr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = 0;

    reply = m_nam->post(nr,args);


}

void FacebookLogin::gotNetworkReply(QNetworkReply *reply) {

    QString result;

    if (reply->error() > 0) {
        qDebug() << "Error number = " << reply->errorString();
    }
    else {
        result.append( reply->readAll() );
    }

    reply->deleteLater();

    // ^%&#^@% Facebook login bug
    if (result.contains("Cookies are not enabled on your browser."))
    {

        //qDebug() << "**** Cookies NOT enabled *******";

        QUrl url("https://login.facebook.com/login.php");
        QByteArray args;
        args.append("email=" + m_userInfo->getEmailAddy() +
                    "&pass=" + m_userInfo->getPass());

        QByteArray exclude("&=");
        QByteArray include;
        args = args.toPercentEncoding(exclude,include,'%');

        QNetworkRequest nr;
        nr.setUrl(url);
        nr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkReply *reply2 = 0;

        reply2 = m_nam->post(nr,args);


    } else if (result.compare("") == 0) {
        //qDebug() << "Result was empty";

        QUrl url("http://www.facebook.com/home.php?");
        QByteArray args;
        args.append("email=" + m_userInfo->getEmailAddy() +
                    "&pass=" + m_userInfo->getPass());

        QByteArray exclude("&=");
        QByteArray include;
        args = args.toPercentEncoding(exclude,include,'%');
        QNetworkRequest nr;
        nr.setRawHeader("User-Agent","Mozilla/5.0 (X11; U; Linux x86_64; cs-CZ; rv:1.9.1.7) Gecko/20100106 Ubuntu/9.10 (karmic) Firefox/3.5.7");
        nr.setUrl(url);
        QNetworkReply *reply2 = 0;

        reply2 = m_nam->post(nr,args);

    } else {

        //qDebug() << result;
        // In theory, we should now be logged in, and have the cookies in our jar
        m_userInfo->setChannel("");
        m_userInfo->setPostFormId("");

        QRegExp rx;

        rx.setPattern("channel(\\d+)");
        if (rx.indexIn(result) != -1) {
            m_userInfo->setChannel(rx.cap(1));
        }

        rx.setPattern("<input type=\"hidden\" id=\"post_form_id\" name=\"post_form_id\" value=\"([^\"]+)");
        if (rx.indexIn(result) != -1) {
            m_userInfo->setPostFormId(rx.cap(1));
        }

        qDebug() << " channel: " << m_userInfo->getChannel()
                << " PostFormId: " << m_userInfo->getPostFormId();

        if ((m_userInfo->getChannel().compare("") == 0) ||
            (m_userInfo->getPostFormId().compare("") == 0))
        {
            // We didn't log in.
            emit loginResults(false);
        }
        else
        {
            emit loginResults(true);
        }

    }


}



} // namespace UTIL
