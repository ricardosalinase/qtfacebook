#include "comments_get.h"

namespace API {
namespace Comments {

Get::Get()
{
    m_methodName = "comments.get";

    QStringList oneOf;
    oneOf.append("xid");
    oneOf.append("object_id");

    requiresOneOf(oneOf);
}

} // namespace Comments
} // namespace API
