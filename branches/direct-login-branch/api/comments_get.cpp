#include "comments_get.h"

namespace API {
namespace Comments {

    Get::Get(QObject *parent) : Method(parent)
{
    QStringList oneOf;
    oneOf.append("xid");
    oneOf.append("object_id");

    requiresOneOf(oneOf);
}

QString Get::getMethodName() {
    return "comments.get";
}

bool Get::startElement(const QString &, const QString &,
                                  const QString &, const QXmlAttributes &) {
    return true;
}

bool Get::endElement(const QString &, const QString &,
                                const QString &) {
    return true;
}



} // namespace Comments
} // namespace API
