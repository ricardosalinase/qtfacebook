#include "FbAlbum.h"

namespace DATA {

FbAlbum::FbAlbum() :
    m_numPhotos(0)
{
}

void FbAlbum::setAlbumId(const QString &albumId) {
    m_albumId = albumId;
}

const QString& FbAlbum::getAlbumId() {
    return m_albumId;
}

void FbAlbum::setOwnerId(const QString &ownerId) {
    m_ownerId = ownerId;
}

const QString& FbAlbum::getOwnerID() {
    return m_ownerId;
}

void FbAlbum::setCoverPID(const QString &coverPID) {
    m_coverPID = coverPID;
}

const QString& FbAlbum::getCoverPID() {
    return m_coverPID;
}

void FbAlbum::setAlbumName(const QString &albumName) {
    m_albumName = albumName;
}

const QString& FbAlbum::getAlbumName() {
    return m_albumName;
}

void FbAlbum::setCreatedTime(const QString &createdTime) {
    m_createdTime = createdTime;
}

const QString& FbAlbum::getCreatedTime() {
    return m_createdTime;
}

void FbAlbum::setModifiedTime(const QString &modifiedTime) {
    m_modifiedTime = modifiedTime;
}

const QString& FbAlbum::getModifiedTime() {
    return m_modifiedTime;
}

void FbAlbum::setDescription(const QString &desc) {
    m_description = desc;
}

const QString& FbAlbum::getDescription() {
    return m_description;
}

void FbAlbum::setLocation(const QString &location) {
    m_location = location;
}

const QString& FbAlbum::getLocation() {
    return m_location;
}

void FbAlbum::setNumPhotos(const QString &numPhotos) {
    m_numPhotos = numPhotos.toInt();
}

int FbAlbum::getNumPhotos() {
    return m_numPhotos;
}

void FbAlbum::setObjectId(const QString &objectId) {
    m_objectId = objectId;
}

const QString& FbAlbum::getObjectId() {
    return m_objectId;
}





} // namespace DATA
