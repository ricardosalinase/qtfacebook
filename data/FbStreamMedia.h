#ifndef FBSTREAMMEDIA_H
#define FBSTREAMMEDIA_H

#include <QUrl>
#include <QString>
#include <QVariant>
#include <QMap>


namespace DATA {

class FbStreamMedia
{
public:
    FbStreamMedia();

    void setType(QString& type);
    QString& getType();
    void setAlt(QString& alt);
    QString& getAlt();
    void setHref(QString& href);
    QUrl& getHref();
    void setSrc(QString& src);
    QUrl& getSrc();
    void setMedia(QVariant *media);
    QVariant * getMedia();
    void setMediaDetail(const QString& key, const QString& value);
    QMap<QString, QString>& getMediaDetail();

protected:
    QString m_type;
    QString m_alt;
    QUrl m_href;
    QUrl m_src;
    QVariant *m_media;
    QMap<QString, QString> m_mediaDetail;
};

} // namespace DATA

#endif // FBSTREAMMEDIA_H
