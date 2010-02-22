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


protected:
    QString m_type;
    QString m_alt;
    QUrl m_href;
    QUrl m_src;
    QVariant *m_media;
};

} // namespace DATA

#endif // FBSTREAMMEDIA_H
