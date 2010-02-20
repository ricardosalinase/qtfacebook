#ifndef FBUSERPICCACHE_H
#define FBUSERPICCACHE_H

#include <QMap>
#include <QUrl>
#include <QString>
#include <QPixmap>

namespace UTIL {

class FbUserPicCollection
{

public:
    FbUserPicCollection();
    void setPic(QUrl& url, QPixmap& p);
    QPixmap * getPic();
    QUrl& getPicUrl();
    void setPicSquare(QUrl& url, QPixmap& p);
    QPixmap * getPicSquare();
    QUrl& getPicSquareUrl();
    void setPicBig(QUrl& url, QPixmap& p);
    QPixmap * getPicBig();
    QUrl& getPicBigUrl();

private:
    QPixmap m_pic;
    QUrl m_picUrl;
    QPixmap m_picSquare;
    QUrl m_picSquareUrl;
    QPixmap m_picBig;
    QUrl m_picBigUrl;

};


class FbUserPicCache
{
public:
    enum PicType { Pic, PicSquare, PicBig };
    static FbUserPicCache * getInstance();

    QPixmap * getPixmap(QString& uid, PicType type);
    QPixmap * getPixmap(QString& uid, PicType type, QUrl& url);
    void cachePixmap(QString &uid, PicType type, QUrl url, QPixmap &pm);

protected:
    FbUserPicCache();
private:
    static FbUserPicCache *m_instance;
    QMap<QString, FbUserPicCollection *> m_cache;

};

} // namespace UTIL

#endif // FBUSERPICCACHE_H
