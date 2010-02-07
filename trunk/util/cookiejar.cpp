
#include "cookiejar.h"

#include <QtCore/QUrl>
#include <QtCore/QDebug>

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

} // end namespace UTIL

