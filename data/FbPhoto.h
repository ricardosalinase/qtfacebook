#ifndef FBPHOTO_H
#define FBPHOTO_H

#include <QString>
#include <QUrl>

#include "FbStreamMedia.h"
#include "data/fbuserinfo.h"
#include "data/FbPageInfo.h"

namespace DATA {

class FbPhoto
{
public:
    FbPhoto();

    void setPhotoId(const QString& pid);
    QString& getPhotoId();
    void setAlbumId(const QString& aid);
    QString& getAlbumId();
    void setOwnerId(const QString& id);
    QString& getOwnerId();
    void setUserInfo(const FbUserInfo *info);
    FbUserInfo& getUserInfo();
    void setPageInfo(const FbPageInfo *info);
    FbPageInfo& getPageInfo();
    void setSrcSmall(const QString& ss);
    QUrl& getSrcSmall();
    void setSrcSmallHeight(int h);
    int getSrcSmallHeight();
    void setSrcSmallWidth(int w);
    int getSrcSmallWidth();
    void setSrcBig(const QString& sb);
    QUrl& getSrcBig();
    void setSrcBigHeight(int h);
    int getSrcBigHeight();
    void setSrcBigWidth(int w);
    int getSrcBigWidth();
    void setSrc(const QString& s);
    QUrl& getSrc();
    void setSrcHeight(int h);
    int getSrcHeight();
    void setSrcWidth(int w);
    int getSrcWidth();
    void setCaption(const QString& caption);
    QString& getCaption();
    void setCreatedTime(const QString& t);
    QString& getCreatedTime();
    void setModifiedTime(const QString& t);
    QString& getModifiedTime();
    void setObjectId(const QString& oid);
    QString& getObjectId();
    bool isUserOwned();
    void setAlbumName(const QString& name);
    QString& getAlbumName();

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
    QString m_albumName;
    FbUserInfo m_userOwnerInfo;
    FbPageInfo m_pageOwnerInfo;
    bool m_isOwnedByUser;
    

};

} // namespace DATA
Q_DECLARE_METATYPE(DATA::FbPhoto)

#endif // FBPHOTO_H


