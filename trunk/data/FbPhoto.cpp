#include "FbPhoto.h"

namespace DATA {

FbPhoto::FbPhoto()
{
}

void FbPhoto::setPhotoId(const QString &pid) {
    m_pid = pid;
}

const QString& FbPhoto::getPhotoId() {
    return m_pid;
}

void FbPhoto::setAlbumId(const QString &aid) {
    m_aid = aid;
}

const QString& FbPhoto::getAlbumId() {
    return m_aid;
}

void FbPhoto::setOwnerId(const QString &id) {
    m_owner = id;
}

const QString& FbPhoto::getOwnerId() {
    return m_owner;
}

void FbPhoto::setSrcSmall(const QString &ss) {
    m_srcSmall = QUrl(ss);
}

const QUrl& FbPhoto::getSrcSmall() {
    return m_srcSmall;
}

void FbPhoto::setSrcSmallHeight(int h) {
    m_srcSmallHeight = h;
}

int FbPhoto::getSrcSmallHeight() {
    return m_srcSmallHeight;
}

void FbPhoto::setSrcSmallWidth(int w) {
    m_srcSmallWidth = w;
}

int FbPhoto::getSrcSmallWidth() {
    return m_srcSmallWidth;
}

void FbPhoto::setSrcBig(const QString &sb) {
    m_srcBig = QUrl(sb);
}

const QUrl& FbPhoto::getSrcBig() {
    return m_srcBig;
}

void FbPhoto::setSrcBigHeight(int h) {
    m_srcBigHeight = h;
}

int FbPhoto::getSrcBigHeight() {
    return m_srcBigHeight;
}

void FbPhoto::setSrcBigWidth(int w) {
    m_srcBigWidth = w;
}

int FbPhoto::getSrcBigWidth() {
    return m_srcBigWidth;
}

void FbPhoto::setSrc(const QString &s) {
    m_src = QUrl(s);
}

const QUrl& FbPhoto::getSrc() {
    return m_src;
}

void FbPhoto::setSrcHeight(int h) {
    m_srcHeight = h;
}

int FbPhoto::getSrcHeight() {
    return m_srcHeight;
}

void FbPhoto::setSrcWidth(int w) {
    m_srcWidth = w;
}

int FbPhoto::getSrcWidth() {
    return m_srcWidth;
}

void FbPhoto::setCaption(const QString &caption) {
    m_caption = caption;
}

const QString& FbPhoto::getCaption() {
    return m_caption;
}

void FbPhoto::setCreatedTime(const QString &t) {
    m_createdTime = t;
}

const QString& FbPhoto::getCreatedTime() {
    return m_createdTime;
}

void FbPhoto::setModifiedTime(const QString &t) {
    m_modifiedTime = t;
}

const QString& FbPhoto::getModifiedTime() {
    return m_modifiedTime;
}

void FbPhoto::setObjectId(const QString &oid) {
    m_objectId = oid;
}

const QString& FbPhoto::getObjectId() {
    return m_objectId;
}

} // namespace DATA
