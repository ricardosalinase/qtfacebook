
#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QtNetwork/QNetworkCookieJar>

class CookieJar : public QNetworkCookieJar
{

    Q_OBJECT

signals:


public:

    CookieJar(QObject *parent = 0);
    ~CookieJar();

    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);


public slots:

private slots:

private:

};

#endif // COOKIEJAR_H

