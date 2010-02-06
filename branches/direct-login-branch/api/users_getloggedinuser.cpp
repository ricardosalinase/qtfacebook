#include "users_getloggedinuser.h"

namespace API {
namespace Users {

    GetLoggedInUser::GetLoggedInUser(QObject *parent) : Method(parent)
{



}
bool GetLoggedInUser::startElement(const QString &, const QString &,
                                  const QString &, const QXmlAttributes &) {
    return true;
}

bool GetLoggedInUser::endElement(const QString &, const QString &,
                                const QString &) {
    return true;
}

QString GetLoggedInUser::getMethodName() {
    return "users.getLoggedInUser";
}



} // namespace Users
} // namespace API
