#include "FbPhotoCache.h"

namespace UTIL {

FbPhotoCache * FbPhotoCache::m_instance = 0;

FbPhotoCache::FbPhotoCache()
{


}

FbPhotoCache * FbPhotoCache::getInstance() {
    if (!m_instance)
        m_instance = new FbPhotoCache();

    return m_instance;

}

QPixmap * FbPhotoCache::getPixmap(QString& pid, PicType type) {

    if (m_cache.contains(pid))
    {
        switch(type)
        {
        case Pic:
            return m_cache[pid]->getPic();
            break;
        case PicSmall:
            return m_cache[pid]->getPicSmall();
            break;
        case PicBig:
            return m_cache[pid]->getPicBig();
            break;
        default:
            return 0;
            break;
        }

    }
    else // pid not in cache at all
    {
        return 0;
    }

}

QPixmap * FbPhotoCache::getPixmap(QString& pid, PicType type, QUrl& url) {

    if (m_cache.contains(pid))
    {
        switch(type)
        {
        case Pic:
            if (m_cache[pid]->getPicUrl() == url)
                return m_cache[pid]->getPic();
            else
                return 0;
            break;
        case PicSmall:
            if (m_cache[pid]->getPicSmallUrl() == url)
                return m_cache[pid]->getPicSmall();
            else
                return 0;
        case PicBig:
            if (m_cache[pid]->getPicBigUrl() == url)
                return m_cache[pid]->getPicBig();
            else return 0;
        default:
            return 0;
        }

    }
    else // pid not in cache at all
    {
        return 0;
    }

}

void FbPhotoCache::cachePixmap(QString &pid, PicType type, QUrl url, QPixmap &pm) {

    if (!m_cache.contains(pid))
    {
        m_cache.insert(pid, new FbPhotoPicCollection());
    }

    switch(type)
    {
    case Pic:
        m_cache[pid]->setPic(url,pm);
        break;
    case PicSmall:
        m_cache[pid]->setPicSmall(url, pm);
        break;
    case PicBig:
        m_cache[pid]->setPicBig(url, pm);
        break;
    default:
        break;
    }


}

FbPhotoPicCollection::FbPhotoPicCollection() {

}

void FbPhotoPicCollection::setPic(const QUrl &url, const QPixmap &p) {

    m_pic = p;
    m_picUrl = url;

}

QPixmap * FbPhotoPicCollection::getPic() const {
    if (m_picUrl.toString().compare("") == 0)
        return 0;
    else
        return new QPixmap(m_pic);
}

const QUrl & FbPhotoPicCollection::getPicUrl() const {
    return m_picUrl;
}

void FbPhotoPicCollection::setPicSmall(const QUrl &url, const QPixmap &p) {

    m_picSmall = p;
    m_picSmallUrl = url;
}


QPixmap * FbPhotoPicCollection::getPicSmall() const {

    if (m_picSmallUrl.toString().compare("") == 0)
        return 0;
    else
        return new QPixmap(m_picSmall);
}

const QUrl & FbPhotoPicCollection::getPicSmallUrl() const {
    return m_picSmallUrl;
}

void FbPhotoPicCollection::setPicBig(const QUrl &url, const QPixmap &p) {

    m_picBig = p;
    m_picBigUrl = url;
}

QPixmap * FbPhotoPicCollection::getPicBig() const {

    if (m_picBigUrl.toString().compare("") == 0)
        return 0;
    else
        return new QPixmap(m_picBig);
}

const QUrl & FbPhotoPicCollection::getPicBigUrl() const {
    return m_picBigUrl;
}

} // namespace UTIL
