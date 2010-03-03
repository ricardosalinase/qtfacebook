#ifndef FBALBUM_H
#define FBALBUM_H

#include <QString>

namespace DATA {

class FbAlbum
{
public:
    FbAlbum();
    void setAlbumId(const QString& albumId);
    const QString& getAlbumId();
    void setOwnerId(const QString& ownerId);
    const QString& getOwnerID();
    void setCoverPID(const QString& coverPID);
    const QString& getCoverPID();
    void setAlbumName(const QString& albumName);
    const QString& getAlbumName();
    void setCreatedTime(const QString& createdTime);
    const QString& getCreatedTime();
    void setModifiedTime(const QString& modifiedTime);
    const QString& getModifiedTime();
    void setDescription(const QString& desc);
    const QString& getDescription();
    void setLocation(const QString& location);
    const QString& getLocation();
    void setNumPhotos(const QString& numPhotos);
    int getNumPhotos();
    void setObjectId(const QString& objectId);
    const QString& getObjectId();


private:
    QString m_albumId;
    QString m_ownerId;
    QString m_coverPID;
    QString m_albumName;
    QString m_createdTime;
    QString m_modifiedTime;
    QString m_description;
    QString m_location;
    int m_numPhotos;
    QString m_objectId;


};

} // namespace DATA

#endif // FBALBUM_H
