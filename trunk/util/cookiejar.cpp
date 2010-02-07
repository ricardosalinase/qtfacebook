
#include "cookiejar.h"

#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QSettings>

/*
QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{

    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i)
    {
        QByteArray value;
        stream >> value;
        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0) {
            qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
        }
        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));
        if (stream.atEnd())
            break;
    }
    return stream;
}
QT_END_NAMESPACE */

namespace UTIL {

CookieJar::CookieJar(QObject *parent)
    : QNetworkCookieJar(parent)
{
}

CookieJar::~CookieJar()
{
}





bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    QUrl u = url;

    QList<QNetworkCookie> cookies = allCookies();
    foreach(QNetworkCookie cookie, cookieList) {
        //qDebug() << cookie;
        cookies += cookie;
    }
    setAllCookies(cookies);
    return true;
}


QList<QByteArray> CookieJar::getRawCookies() {

    QList<QByteArray> rc;
    QList<QNetworkCookie> cookies = allCookies();
    foreach(QNetworkCookie cookie, cookies) {
        rc += cookie.toRawForm();
    }
    return rc;
}

void CookieJar::createJarFromRaw(QList<QByteArray> rc) {

    QList<QNetworkCookie> cookies = allCookies();
    foreach(QByteArray rawCookie, rc) {
        QList<QNetworkCookie> nc = QNetworkCookie::parseCookies(rawCookie);
        cookies += nc.takeFirst();
    }
    setAllCookies(cookies);

}

} // end namespace UTIL

