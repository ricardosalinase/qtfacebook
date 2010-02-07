
#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QtNetwork/QNetworkCookieJar>
#include <QMetaType>

namespace UTIL {

class CookieJar : public QNetworkCookieJar
{

    Q_OBJECT

signals:


public:

    CookieJar(QObject *parent = 0);
    ~CookieJar();

    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);
    void save();
    void load();


public slots:

private slots:

private:

};

} // end namespace UTIL
#endif // COOKIEJAR_H

