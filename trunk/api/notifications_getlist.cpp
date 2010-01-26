#include "notifications_getlist.h"

namespace API {
namespace Notifications {

GetList::GetList(QObject *parent) : Method(parent)
{


}

bool GetList::startElement(const QString &, const QString &,
                                  const QString &, const QXmlAttributes &) {
    return true;
}

bool GetList::endElement(const QString &, const QString &,
                                const QString &) {
    return true;
}

QString GetList::getMethodName() {
    return "notifications.getList";
}



} // namespace Notifications
} // namespace API
