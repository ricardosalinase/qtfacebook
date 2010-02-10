#include "buddy.h"

namespace DATA {

Buddy::Buddy()
{
}

QString Buddy::getUID() {
    return m_uid;
}

void Buddy::setUID(QString id) {
    m_uid = id;
}

QString Buddy::getFirstName() {
    return m_firstName;
}

void Buddy::setFirstName(QString fn) {
    m_firstName = fn;
}

QString Buddy::getFullName() {
    return m_fullName;
}

void Buddy::setFullName(QString fn) {
    m_fullName = fn;
}

QUrl Buddy::getThumbUrl() {
    return m_thumbUrl;
}

void Buddy::setThumbUrl(QString url) {
    m_thumbUrl = QUrl(url);
}

bool Buddy::isIdle() {
    return m_idle;
}

void Buddy::isIdle(bool idle) {
    m_idle = idle;
}

bool Buddy::isInList(QString listId) {
    if (m_listIds.contains(listId))
        return true;
    else
        return false;
}

QList<QString> Buddy::getListIds() {
    return m_listIds;
}

void Buddy::addToList(QString listId) {
    m_listIds.append(listId);
}

void Buddy::addtoList(QStringList ids) {
    m_listIds.append(ids);
}





} // namespace DATA
