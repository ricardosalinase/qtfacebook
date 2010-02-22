#include "FbStreamAttachment.h"

namespace DATA {

FbStreamAttachment::FbStreamAttachment()
{
}

void FbStreamAttachment::setName(QString &name) {
    m_name = name;
}

QString& FbStreamAttachment::getName() {
    return m_name;
}

void FbStreamAttachment::setHref(QString &href) {
    m_href = QUrl(href);
}

QUrl& FbStreamAttachment::getHref() {
    return m_href;
}

void FbStreamAttachment::setDescription(QString &desc) {
    m_desc = desc;
}

QString& FbStreamAttachment::getDescription() {
    return m_desc;
}

void FbStreamAttachment::setCaption(QString &caption) {
    m_caption = caption;
}


QString& FbStreamAttachment::getCaption() {
    return m_caption;
}

void FbStreamAttachment::setIcon(QString &icon) {
    m_icon = QUrl(icon);
}

QUrl& FbStreamAttachment::getIcon() {
    return m_icon;
}

void FbStreamAttachment::setFbObjectType(QString &type) {
    m_objType = type;
}

QString& FbStreamAttachment::getFbObjectType() {
    return m_objType;
}

void FbStreamAttachment::addMedia(DATA::FbStreamMedia *media) {
    m_media.append(media);
}

QList<DATA::FbStreamMedia *>& FbStreamAttachment::getMedia() {
    return m_media;
}


} // namespace DATA
