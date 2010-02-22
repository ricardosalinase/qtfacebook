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
    m_src = QUrl(src);
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

} // namespace DATA
