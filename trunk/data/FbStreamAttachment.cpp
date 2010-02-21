#include "FbStreamAttachment.h"

namespace DATA {

FbStreamAttachment::FbStreamAttachment()
{
}

void FbStreamAttachment::setName(const QString &name) {
    m_name = name;
}

const QString& FbStreamAttachment::getName() {
    return m_name;
}

void FbStreamAttachment::setHref(const QString &href) {
    m_href = QUrl(href);
}

const QUrl& FbStreamAttachment::getHref() {
    return m_href;
}

void FbStreamAttachment::setDescription(const QString &desc) {
    m_desc = desc;
}

const QString& FbStreamAttachment::getDescription() {
    return m_desc;
}

void FbStreamAttachment::setCaption(const QString &caption) {
    m_caption = caption;
}


const QString& FbStreamAttachment::getCaption() {
    return m_caption;
}

void FbStreamAttachment::setIcon(const QString &icon) {
    m_icon = QUrl(icon);
}

const QUrl& FbStreamAttachment::getIcon() {
    return m_icon;
}

void FbStreamAttachment::setFbObjectType(const QString &type) {
    m_objType = type;
}

const QString& FbStreamAttachment::getFbObjectType() {
    return m_objType;
}

void FbStreamAttachment::addMedia(const DATA::FbStreamMedia *media) {
    m_media.append(media);
}

const QList<const DATA::FbStreamMedia *>& FbStreamAttachment::getMedia() {
    return m_media;
}


} // namespace DATA
