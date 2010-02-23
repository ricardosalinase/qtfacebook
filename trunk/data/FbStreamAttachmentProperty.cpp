#include "FbStreamAttachmentProperty.h"

#include <QByteArray>

namespace DATA {

FbStreamAttachmentProperty::FbStreamAttachmentProperty()
{
}

void FbStreamAttachmentProperty::setName(QString &name) {
    m_name = name;
}

QString& FbStreamAttachmentProperty::getName() {
    return m_name;
}

void FbStreamAttachmentProperty::setText(QString &text) {
    m_text = text;
}

QString& FbStreamAttachmentProperty::getText() {
    return m_text;
}

void FbStreamAttachmentProperty::setHref(QString &href) {
    QByteArray url;
    url.append(href);
    m_href.setEncodedUrl(url);
}

QUrl& FbStreamAttachmentProperty::getHref() {
    return m_href;
}


} // namespace DATA
