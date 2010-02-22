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

    void setPhotoId(QString& pid);
    QString& getPhotoId();
    void setAlbumId(QString& aid);
    QString& getAlbumId();
    void setOwnerId(QString& id);
    QString& getOwnerId();
    void setSrcSmall(QString& ss);
    QUrl& getSrcSmall();
    void setSrcSmallHeight(int h);
    int getSrcSmallHeight();
    void setSrcSmallWidth(int w);
    int getSrcSmallWidth();
    void setSrcBig(QString& sb);
    QUrl& getSrcBig();
    void setSrcBigHeight(int h);
    int getSrcBigHeight();
    void setSrcBigWidth(int w);
    int getSrcBigWidth();
    void setSrc(QString& s);
    QUrl& getSrc();
    void setSrcHeight(int h);
    int getSrcHeight();
    void setSrcWidth(int w);
    int getSrcWidth();
    void setCaption(QString& caption);
    QString& getCaption();
    void setCreatedTime(QString& t);
    QString& getCreatedTime();
    void setModifiedTime(QString& t);
    QString& getModifiedTime();
    void setObjectId(QString& oid);
    QString& getObjectId();


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


