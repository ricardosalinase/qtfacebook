#ifndef FBPHOTOCACHE_H
#define FBPHOTOCACHE_H

#include <QMap>
#include <QUrl>
#include <QString>
#include <QPixmap>

namespace UTIL {

class FbPhotoPicCollection
{
public:
    FbPhotoPicCollection();
    void setPic(const QUrl& url, const QPixmap& p);
    QPixmap * getPic() const;
    const QUrl& getPicUrl() const;
    void setPicSmall(const QUrl&, const QPixmap& p);
    QPixmap * getPicSmall() const;
    const QUrl& getPicSmallUrl() const;
    void setPicBig(const QUrl& url, const QPixmap& p);
    QPixmap * getPicBig() const;
    const QUrl& getPicBigUrl() const;

private:
    QPixmap m_pic;
    QUrl m_picUrl;
    QPixmap m_picSmall;
    QUrl m_picSmallUrl;
    QPixmap m_picBig;
    QUrl m_picBigUrl;
};


class FbPhotoCache
{
public:
    enum PicType { Pic, PicSmall, PicBig };
    static FbPhotoCache * getInstance();

    QPixmap * getPixmap(QString& pid, PicType type);
    QPixmap * getPixmap(QString& pid, PicType type, QUrl& url);
    void cachePixmap(QString &pid, PicType type, QUrl url, QPixmap &pm);

protected:
    FbPhotoCache();

private:
    static FbPhotoCache *m_instance;
    QMap<QString, FbPhotoPicCollection *> m_cache;

};

} // namespace UTIL

#endif // FBPHOTOCACHE_H
