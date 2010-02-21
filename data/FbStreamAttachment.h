#ifndef FBSTREAMATTACHMENT_H
#define FBSTREAMATTACHMENT_H

#include <QString>
#include <QUrl>
#include <QList>

#include "data/FbStreamMedia.h"

namespace DATA {

class FbStreamAttachment
{
public:
    FbStreamAttachment();

    void setName(const QString& name);
    const QString& getName();
    void setHref(const QString& href);
    const QUrl& getHref();
    void setDescription(const QString& desc);
    const QString& getDescription();
    void setCaption(const QString& caption);
    const QString& getCaption();
    void setIcon(const QString& icon);
    const QUrl& getIcon();
    void setFbObjectType(const QString& type);
    const QString& getFbObjectType();
    void addMedia(const DATA::FbStreamMedia *media);
    const QList<const DATA::FbStreamMedia *>& getMedia();

private:
    QString m_name;
    QUrl m_href;
    QString m_desc;
    QString m_caption;
    QUrl m_icon;
    QString m_objType;
    QList<const DATA::FbStreamMedia *> m_media;


};

} // namespace DATA

#endif // FBSTREAMATTACHMENT_H
