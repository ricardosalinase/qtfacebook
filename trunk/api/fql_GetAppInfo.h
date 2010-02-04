#ifndef FQL_GETAPPINFO_H
#define FQL_GETAPPINFO_H

#include "apimethod.h"
#include "data/appinfo.h"

using namespace DATA;

namespace API {
namespace FQL {

class GetAppInfo : public API::Method
{
public:
    explicit GetAppInfo(QObject *parent = 0);
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    QString getMethodName();
    QMap<QString, AppInfo*> * getAppInfoMap();



protected:
    bool prepare();

private:
    QMap<QString, AppInfo* > *m_appInfo;
    AppInfo *m_currentAppInfo;

};

} // namespace FQL
} // namespace API

#endif // FQL_GETAPPINFO_H
