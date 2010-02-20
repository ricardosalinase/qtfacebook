#include "FbPageInfo.h"

namespace DATA {

FbPageInfo::FbPageInfo()
{
}


void FbPageInfo::setPageId(const QString &id) {
    m_pageId = id;
}

QString& FbPageInfo::getPageId() {
    return m_pageId;
}

void FbPageInfo::setName(const QString &name) {
    m_name = name;
}

QString& FbPageInfo::getName() {
    return m_name;
}

void FbPageInfo::setPic(const QString &pic) {
    m_pic = QUrl(pic);
}

QUrl& FbPageInfo::getPic() {
    return m_pic;
}

void FbPageInfo::setPicSquare(const QString &ps)  {
    m_picSquare = QUrl(ps);
}

QUrl& FbPageInfo::getPicSquare() {
    return m_picSquare;
}

void FbPageInfo::setPicBig(const QString &pb) {
    m_picBig = QUrl(pb);
}

QUrl& FbPageInfo::getPicBig() {
    return m_picBig;
}

} // namespace DATA

