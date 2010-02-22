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

    void setName(QString& name);
    QString& getName();
    void setHref(QString& href);
    QUrl& getHref();
    void setDescription(QString& desc);
    QString& getDescription();
    void setCaption(QString& caption);
    QString& getCaption();
    void setIcon(QString& icon);
    QUrl& getIcon();
    void setFbObjectType(QString& type);
    QString& getFbObjectType();
    void addMedia(DATA::FbStreamMedia *media);
    QList<DATA::FbStreamMedia *>& getMedia();

private:
    QString m_name;
    QUrl m_href;
    QString m_desc;
    QString m_caption;
    QUrl m_icon;
    QString m_objType;
    QList<DATA::FbStreamMedia *> m_media;


};

} // namespace DATA

#endif // FBSTREAMATTACHMENT_H
