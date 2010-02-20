#include "fbuserinfo.h"

namespace DATA {

FbUserInfo::FbUserInfo()
{
}

void FbUserInfo::setUID(QString id) {
    m_uid = id;
}

QString& FbUserInfo::getUID() {
    return m_uid;
}

void FbUserInfo::setName(QString name) {
    m_name = name;
}

QString& FbUserInfo::getName() {
    return m_name;
}

void FbUserInfo::setPic(QString pic) {
    m_pic = QUrl(pic);
}

QUrl& FbUserInfo::getPic() {
    return m_pic;
}

void FbUserInfo::setPicSquare(QString ps) {
    m_picSquare = QUrl(ps);
}

QUrl& FbUserInfo::getPicSquare() {
    return m_picSquare;
}

void FbUserInfo::setPicBig(QString pb) {
    m_picBig = QUrl(pb);
}

QUrl& FbUserInfo::getPicBig() {
    return m_picBig;
}

} // namespace DATA
