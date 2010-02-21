#ifndef FBSTREAMMEDIA_H
#define FBSTREAMMEDIA_H

#include <QUrl>
#include <QString>
#include <QVariant>


namespace DATA {

class FbStreamMedia
{
public:
    FbStreamMedia();

    void setType(const QString& type);
    const QString& getType();
    void setAlt(const QString& alt);
    const QString& getAlt();
    void setHref(const QString& href);
    const QUrl& getHref();
    void setSrc(const QString& src);
    const QUrl& getSrc();
    void setMedia(const QVariant *media);
    const QVariant * getMedia();


protected:
    QString m_type;
    QString m_alt;
    QUrl m_href;
    QUrl m_src;
    const QVariant *m_media;
};

} // namespace DATA

#endif // FBSTREAMMEDIA_H
