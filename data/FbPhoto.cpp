#include "FbPhoto.h"

namespace DATA {

FbPhoto::FbPhoto()
{
}

void FbPhoto::setPhotoId(QString &pid) {
    m_pid = pid;
}

QString& FbPhoto::getPhotoId() {
    return m_pid;
}

void FbPhoto::setAlbumId(QString &aid) {
    m_aid = aid;
}

QString& FbPhoto::getAlbumId() {
    return m_aid;
}

void FbPhoto::setOwnerId(QString &id) {
    m_owner = id;
}

QString& FbPhoto::getOwnerId() {
    return m_owner;
}

void FbPhoto::setSrcSmall(QString &ss) {
    m_srcSmall = QUrl(ss);
}

QUrl& FbPhoto::getSrcSmall() {
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

void FbPhoto::setSrcBig(QString &sb) {
    m_srcBig = QUrl(sb);
}

QUrl& FbPhoto::getSrcBig() {
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

void FbPhoto::setSrc(QString &s) {
    m_src = QUrl(s);
}

QUrl& FbPhoto::getSrc() {
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

void FbPhoto::setCaption(QString &caption) {
    m_caption = caption;
}

QString& FbPhoto::getCaption() {
    return m_caption;
}

void FbPhoto::setCreatedTime(QString &t) {
    m_createdTime = t;
}

QString& FbPhoto::getCreatedTime() {
    return m_createdTime;
}

void FbPhoto::setModifiedTime(QString &t) {
    m_modifiedTime = t;
}

QString& FbPhoto::getModifiedTime() {
    return m_modifiedTime;
}

void FbPhoto::setObjectId(QString &oid) {
    m_objectId = oid;
}

QString& FbPhoto::getObjectId() {
    return m_objectId;
}

} // namespace DATA
