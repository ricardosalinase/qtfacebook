#include "notifications_markread.h"
#include <QDebug>

namespace API {
namespace Notifications {

MarkRead::MarkRead(QObject *parent) :
            Method(parent),
            m_succeeded(false)
{
    requires("notification_ids");
}
bool MarkRead::startElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                            const QString &qName, const QXmlAttributes &/*attributes*/) {

    qDebug() << "Start: " << qName;
    m_currentText.clear();
    return true;
}

bool MarkRead::endElement(const QString &/*namespaceURI*/, const QString &/*localName*/,
                          const QString &qName) {

    if (qName == "notifications_markRead_response")
        (m_currentText.compare("1") == 0) ? m_succeeded = true : m_succeeded = false;

    qDebug() << "End: " << qName << " " <<  m_currentText;
    return true;
}

QString MarkRead::getMethodName() {
    return "notifications.markRead";
}

bool MarkRead::successful() {
    return m_succeeded;
}

QString MarkRead::getNotificationIds() {
    return m_argMap["notification_ids"].toString();
}


} // namespace Notifications
} // namespace API
