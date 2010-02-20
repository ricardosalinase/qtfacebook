#include "fbuserpiccache.h"

namespace UTIL {

FbUserPicCache * FbUserPicCache::m_instance = 0;

FbUserPicCache::FbUserPicCache()
{

}

FbUserPicCache * FbUserPicCache::getInstance() {
    if (!m_instance)
        m_instance = new FbUserPicCache();

    return m_instance;

}

QPixmap * FbUserPicCache::getPixmap(QString& uid, PicType type) {

    if (m_cache.contains(uid))
    {
        switch(type)
        {
        case Pic:
            return m_cache[uid]->getPic();
            break;
        case PicSquare:
            return m_cache[uid]->getPicSquare();
            break;
        case PicBig:
            return m_cache[uid]->getPicBig();
            break;
        default:
            return 0;
            break;
        }

    }
    else // uid not in cache at all
    {
        return 0;
    }

}

QPixmap * FbUserPicCache::getPixmap(QString& uid, PicType type, QUrl& url) {

    if (m_cache.contains(uid))
    {
        switch(type)
        {
        case Pic:
            if (m_cache[uid]->getPicUrl() == url)
                return m_cache[uid]->getPic();
            else
                return 0;
            break;
        case PicSquare:
            if (m_cache[uid]->getPicSquareUrl() == url)
                return m_cache[uid]->getPicSquare();
            else
                return 0;
        case PicBig:
            if (m_cache[uid]->getPicBigUrl() == url)
                return m_cache[uid]->getPicBig();
            else return 0;
        default:
            return 0;
        }

    }
    else // uid not in cache at all
    {
        return 0;
    }

}


void FbUserPicCache::cachePixmap(QString &uid, PicType type, QUrl url, QPixmap &pm) {

    if (!m_cache.contains(uid))
    {
        m_cache.insert(uid, new FbUserPicCollection());
    }

    switch(type)
    {
    case Pic:
        m_cache[uid]->setPic(url,pm);
        break;
    case PicSquare:
        m_cache[uid]->setPicSquare(url, pm);
        break;
    case PicBig:
        m_cache[uid]->setPicBig(url, pm);
        break;
    default:
        break;
    }


}

FbUserPicCollection::FbUserPicCollection() {

}

void FbUserPicCollection::setPic(QUrl &url, QPixmap &p) {

    m_pic = p;
    m_picUrl = url;

}

QPixmap * FbUserPicCollection::getPic() {
    if (m_picUrl.toString().compare("") == 0)
        return 0;
    else
        return new QPixmap(m_pic);
}

QUrl & FbUserPicCollection::getPicUrl() {
    return m_picUrl;
}

void FbUserPicCollection::setPicSquare(QUrl &url, QPixmap &p) {

    m_picSquare = p;
    m_picSquareUrl = url;
}


QPixmap * FbUserPicCollection::getPicSquare() {

    if (m_picSquareUrl.toString().compare("") == 0)
        return 0;
    else
        return new QPixmap(m_picSquare);
}

QUrl & FbUserPicCollection::getPicSquareUrl() {
    return m_picSquareUrl;
}

void FbUserPicCollection::setPicBig(QUrl &url, QPixmap &p) {

    m_picBig = p;
    m_picBigUrl = url;
}

QPixmap * FbUserPicCollection::getPicBig() {

    if (m_picBigUrl.toString().compare("") == 0)
        return 0;
    else
        return new QPixmap(m_picBig);
}

QUrl & FbUserPicCollection::getPicBigUrl() {
    return m_picBigUrl;
}


} // namespace UTIL
