#ifndef FBPHOTO_H
#define FBPHOTO_H

#include <QString>
#include <QUrl>

#include "FbStreamMedia.h"

namespace DATA {

class FbPhoto
{
public:
    FbPhoto();

    void setPhotoId(const QString& pid);
    const QString& getPhotoId();
    void setAlbumId(const QString& aid);
    const QString& getAlbumId();
    void setOwnerId(const QString& id);
    const QString& getOwnerId();
    void setSrcSmall(const QString& ss);
    const QUrl& getSrcSmall();
    void setSrcSmallHeight(int h);
    int getSrcSmallHeight();
    void setSrcSmallWidth(int w);
    int getSrcSmallWidth();
    void setSrcBig(const QString& sb);
    const QUrl& getSrcBig();
    void setSrcBigHeight(int h);
    int getSrcBigHeight();
    void setSrcBigWidth(int w);
    int getSrcBigWidth();
    void setSrc(const QString& s);
    const QUrl& getSrc();
    void setSrcHeight(int h);
    int getSrcHeight();
    void setSrcWidth(int w);
    int getSrcWidth();
    void setCaption(const QString& caption);
    const QString& getCaption();
    void setCreatedTime(const QString& t);
    const QString& getCreatedTime();
    void setModifiedTime(const QString& t);
    const QString& getModifiedTime();
    void setObjectId(const QString& oid);
    const QString& getObjectId();


private:
    QString m_pid;
    QString m_aid;
    QString m_owner;
    QUrl m_srcSmall;
    int m_srcSmallHeight;
    int m_srcSmallWidth;
    QUrl m_srcBig;
    int m_srcBigHeight;
    int m_srcBigWidth;
    QUrl m_src;
    int m_srcHeight;
    int m_srcWidth;
    QString m_caption;
    QString m_createdTime;
    QString m_modifiedTime;
    QString m_objectId;

};

} // namespace DATA
Q_DECLARE_METATYPE(DATA::FbPhoto)

#endif // FBPHOTO_H


