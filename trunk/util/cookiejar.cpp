
#include "cookiejar.h"

#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QSettings>


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
QT_END_NAMESPACE

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

// Total hack to get a copy of the cookie jar into another thread.
void CookieJar::save() {
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    QList<QNetworkCookie> cookies = allCookies();
    QSettings settings("qtFacebook","qtFacebook");
    settings.setValue("Cookies",qVariantFromValue<QList<QNetworkCookie> >(cookies) );
}

void CookieJar::load() {
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    QSettings settings("qtFacebook","qtFacebook");
    setAllCookies(qvariant_cast<QList<QNetworkCookie> >(settings.value("cookies")));
}

} // end namespace UTIL

