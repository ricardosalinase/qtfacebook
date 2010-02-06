#include "notifications_get.h"

namespace API {
namespace Notifications {

    Get::Get(QObject *parent) : Method(parent)
{


}

QString Get::getMethodName() {
    return "notifications.get";
}

bool Get::startElement(const QString &, const QString &,
                                  const QString &, const QXmlAttributes &) {
    return true;
}

bool Get::endElement(const QString &, const QString &,
                                const QString &) {
    return true;
}



} // namespace Notifications
} // namespace API
