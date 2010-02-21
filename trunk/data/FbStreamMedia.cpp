#include "FbStreamMedia.h"

namespace DATA {

FbStreamMedia::FbStreamMedia()
{
}

void FbStreamMedia::setType(const QString &type) {
    m_type = type;
}

const QString& FbStreamMedia::getType() {
    return m_type;
}

void FbStreamMedia::setAlt(const QString &alt) {
    m_alt = alt;
}

const QString& FbStreamMedia::getAlt() {
    return m_alt;
}

void FbStreamMedia::setHref(const QString &href) {
    m_href = QUrl(href);
}

const QUrl& FbStreamMedia::getHref() {
    return m_href;
}

void FbStreamMedia::setSrc(const QString &src) {
    m_src = QUrl(src);
}

const QUrl& FbStreamMedia::getSrc() {
    return m_src;
}

void FbStreamMedia::setMedia(const QVariant *media) {
    m_media = media;
}

const QVariant * FbStreamMedia::getMedia() {
    return m_media;
}

} // namespace DATA
