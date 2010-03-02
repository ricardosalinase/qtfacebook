#include "FbStreamMedia.h"

#include <QDebug>

namespace DATA {

FbStreamMedia::FbStreamMedia()
{
}

void FbStreamMedia::setType(QString &type) {
    m_type = type;
}

QString& FbStreamMedia::getType() {
    return m_type;
}

void FbStreamMedia::setAlt(QString &alt) {
    m_alt = alt;
}

QString& FbStreamMedia::getAlt() {
    return m_alt;
}

void FbStreamMedia::setHref(QString &href) {
    m_href = QUrl(href);
}

QUrl& FbStreamMedia::getHref() {
    return m_href;
}

void FbStreamMedia::setSrc(QString& src) {
    QByteArray encodedUrl;
    encodedUrl.append(src);
    m_src.setEncodedUrl(encodedUrl);
}

QUrl& FbStreamMedia::getSrc() {
    return m_src;
}

void FbStreamMedia::setMedia(QVariant *media) {
    m_media = media;
}

QVariant * FbStreamMedia::getMedia() {
    return m_media;
}

void FbStreamMedia::setMediaDetail(const QString &key, const QString &value) {
    m_mediaDetail.insert(key,value);
}

QMap<QString, QString>& FbStreamMedia::getMediaDetail() {
    return m_mediaDetail;
}

} // namespace DATA
