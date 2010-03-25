#include "users_GetLoggedInUser.h"

namespace API {
namespace Users {

    GetLoggedInUser::GetLoggedInUser(QObject *parent) : Method(parent)
{



}
bool GetLoggedInUser::startElement(const QString &, const QString &,
                                  const QString &, const QXmlAttributes &) {
    m_currentText.clear();
    return true;
}

bool GetLoggedInUser::endElement(const QString &, const QString &,
                                const QString &qName) {

    if (qName == "users_getLoggedInUser_response")
        m_uid = m_currentText;

    return true;
}

QString GetLoggedInUser::getMethodName() {
    return "users.getLoggedInUser";
}

QString GetLoggedInUser::getUID()
{
    return m_uid;
}

} // namespace Users
} // namespace API

/* <?xml version="1.0" encoding="UTF-8"?>
<error_response xmlns="http://api.facebook.com/1.0/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://api.facebook.com/1.0/ http://api.facebook.com/1.0/facebook.xsd">
  <error_code>102</error_code>
  <error_msg>Session key invalid or no longer valid</error_msg>
  <request_args list="true">
    <arg>
      <key>method</key>
      <value>users.getLoggedInUser</value>
    </arg>
    <arg>
      <key>api_key</key>
      <value>61cecf6f7ee5528d294e1d6bf675f424</value>
    </arg>
    <arg>
      <key>v</key>
      <value>1.0</value>
    </arg>
    <arg>
      <key>session_key</key>
      <value>e696c698d4cdb4e451e14283-1082239928</value>
    </arg>
    <arg>
      <key>call_id</key>
      <value>1269534447</value>
    </arg>
    <arg>
      <key>sig</key>
      <value>56f181ccdb4bdc2b86819509b4732275</value>
    </arg>
  </request_args>
</error_response>
*/
