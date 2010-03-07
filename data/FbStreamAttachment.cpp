#include "FbStreamAttachment.h"

namespace DATA {

FbStreamAttachment::FbStreamAttachment() :
    m_attachmentIsEmpty(true)
{
}

void FbStreamAttachment::setName(QString &name) {
    m_name = name;
    m_attachmentIsEmpty = false;
}

QString& FbStreamAttachment::getName() {
    return m_name;
}

void FbStreamAttachment::setHref(QString &href) {
    m_href = QUrl(href);
    m_attachmentIsEmpty = false;
}

QUrl& FbStreamAttachment::getHref() {
    return m_href;
}

void FbStreamAttachment::setDescription(QString &desc) {
    m_desc = desc;
    if (desc.compare("") != 0)
        m_attachmentIsEmpty = false;
}

QString& FbStreamAttachment::getDescription() {
    return m_desc;
}

void FbStreamAttachment::setCaption(QString &caption) {
    m_caption = caption;
    m_attachmentIsEmpty = false;
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
    m_attachmentIsEmpty = false;
}

QString& FbStreamAttachment::getFbObjectType() {
    return m_objType;
}

void FbStreamAttachment::setFbObjectId(const QString &id) {
    m_objId = id;
}

const QString& FbStreamAttachment::getFbObjectId() {
    return m_objId;
}

void FbStreamAttachment::addMedia(DATA::FbStreamMedia *media) {
    m_media.append(media);
    m_attachmentIsEmpty = false;
}

QList<DATA::FbStreamMedia *>& FbStreamAttachment::getMedia() {
    return m_media;
}

void FbStreamAttachment::addProperty(DATA::FbStreamAttachmentProperty *p) {
    m_properties.append(p);
    m_attachmentIsEmpty = false;
}

QList<DATA::FbStreamAttachmentProperty *>& FbStreamAttachment::getProperties() {
    return m_properties;
}

bool FbStreamAttachment::isEmpty() {
    return m_attachmentIsEmpty;
}

} // namespace DATA
