#include "apimethod.h"

#include <sys/time.h>
#include <stdio.h>

namespace API {

Method::Method(UserInfo *info, QNetworkAccessManager *manager, QObject *parent) :
    QObject(parent),
    m_userInfo(info),
    m_manager(manager)
{

}

Method::~Method() {

}

/*************************************************************************************/
/* The Observer class contains a single pure virtual function (processMethodResults) */
/* The reason I went this way is because once the QNetworkAccessManager returns the  */
/* QNetworkReply this object is no longer necessary. By requiring that all callers   */
/* inherit from Oberver we have a nice interface connecting them together and this   */
/* object can be safely disposed of.                                                 */
/*************************************************************************************/
bool Method::execute(Observer *observer) {

    if (!validateArgs())
        return false;

    // pull all the key/value pairs from the QMap and do the magic Facebook requires to
    // make an API call
    m_argMap.insert("method", getMethodName());
    m_argMap.insert("api_key", m_userInfo->getApiKey());
    m_argMap.insert("v","1.0");
    m_argMap.insert("session_key",m_userInfo->getSessionKey());

    // This is going to have to be changed to something with greater than
    // second resolution in the main client. Is there anything portable across
    // Mac/Win/Linux? Doubtful, have to build compile options for it.
    time_t tm = time(0);
    char callId[30];
    sprintf(callId, "%ul", tm);
    m_argMap.insert("call_id",callId);



    QNetworkReply *reply;

    connect(reply, SIGNAL(finished()),
            observer, SLOT(processMethodResults(QNetworkReply*)));

    return true;

}

// virtual
bool Method::validateArgs() {
    return true;
}


} // namespace API
