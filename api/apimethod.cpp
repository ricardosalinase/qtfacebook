#include "apimethod.h"

#include <sys/time.h>
#include <stdio.h>

#include <QDebug>
#include <QCryptographicHash>
#include <QUrl>
#include <QNetworkRequest>
#include <QXmlInputSource>

namespace API {

Method::Method(QObject *parent) :
    QObject(parent)
{

}

Method::~Method() {

}

void Method::setAccessManager(QNetworkAccessManager *manager) {
    m_manager = manager;
}

void Method::setUserInfo(UserInfo *userInfo) {
    m_userInfo = userInfo;
}

QString Method::getErrorStr() {
    return m_errStr;
}

void Method::setArgument(QString arg, int value) {
    m_argMap.insert(arg, value);
}

void Method::setArgument(QString arg, QString value) {
    m_argMap.insert(arg, value);
}

void Method::setArgument(QString arg, QStringList value) {
    m_argMap.insert(arg, value);
}


bool Method::execute() {

    if (!prepare())
        return false;

    if (!validate())
        return false;


    // THese are always constant with API requests
    if (getMethodName().startsWith("fql.query",Qt::CaseInsensitive))
        m_argMap.insert("method","fql.query");
    else if (getMethodName().startsWith("fql.multiquery",Qt::CaseInsensitive))
        m_argMap.insert("method","fql.multiquery");
    else
        m_argMap.insert("method", getMethodName());

    m_argMap.insert("api_key", m_userInfo->getApiKey());
    m_argMap.insert("v","1.0");
    m_argMap.insert("session_key",m_userInfo->getSessionKey());

    // This is going to have to be changed to something with greater than
    // second resolution (miliseconds or microseconds). Is there anything portable across
    // Mac/Win/Linux? Doubtful, have to build compile options for it.
    time_t tm = time(0);
    //char callId[30];
    //sprintf(callId, "%lld", tm);
    m_argMap.insert("call_id",(qlonglong)tm);


    // pull all the key/value pairs from the QMap and do the magic Facebook requires to
    // make an API call

    /********** From the Facebook API docs ****************************************
    The signature can be generated by calling generate_sig in facebook.php.
    generate_sig takes two parameters: an array of arg=val pairs and your app secret.
    The signature can also be constructed using the following algorithm
    (after all the other arguments have been determined):

    args = array of args to the request, not counting sig, formatted in non-urlencoded arg=val pairs
    sorted_array = alphabetically_sort_array_by_keys(args);
    request_str = concatenate_in_order(sorted_array);
    signature = md5(concatenate(request_str, secret))
    ***********************************************************************************/

    // In a way, this is really retarded. Facebook wants an MD5 hash of the args with no
    // serparators. You also need a set of args with '&' separated url encoded
    // key/value pairs for the actual post

    QByteArray sigByteArray;
    QByteArray postArgs;

    // QMap is automatically sorted by keys
    QMapIterator<QString, QVariant> i(m_argMap);
    while (i.hasNext()) {
         i.next();
         sigByteArray.append(i.key() + "=" + i.value().toString() );
         postArgs.append(i.key() + "=" + i.value().toString() + "&");
    }

    sigByteArray.append(m_userInfo->getSecret());

    QByteArray sig = QCryptographicHash::hash(sigByteArray,QCryptographicHash::Md5 );

    postArgs.append("sig=");
    postArgs.append(sig.toHex());

    QByteArray exclude("&=");
    QByteArray include;
    postArgs = postArgs.toPercentEncoding(exclude,include,'%');

    // qDebug() << postArgs;

    QUrl url("http://api.facebook.com/restserver.php");

    QNetworkRequest nr;
    nr.setUrl(url);
    nr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = 0;

    m_reply = m_manager->post(nr,postArgs);

    if (m_reply != 0) {
        connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotReply(QNetworkReply*)));
        return true;
    }
    else {
        m_errStr = "The reply was null";
        return false;
    }
}

bool Method::prepare() {
    return true;
}

// Go through the requiredArgs list and make user they all exist in the args map

bool Method::validate() {

    bool allGood = true;

    for (int i = 0; i < m_requiredArgs.size() ; i++) {
        if (m_requiredArgs.at(i).type() == QVariant::String)
            allGood &= m_argMap.contains(m_requiredArgs.at(i).toString());
        else if (m_requiredArgs.at(i).type() == QVariant::List) {
            bool hasAtLeastOne = false;
            for (int j = 0; j < m_requiredArgs.at(i).toList().size(); j++) {
                hasAtLeastOne |= m_argMap.contains(m_requiredArgs.at(i).toList().at(j).toString());
            }
            allGood &= hasAtLeastOne;
        }
    }

    if (!allGood) {
        m_errStr = "Missing Arguments; required:" + getRequiredArgsString();
        return false;
    }

    if (getMethodName().compare("") == 0) {
        m_errStr = "Missing method name!";
        return false;
    }




    return true;
}

QString Method::getRequiredArgsString() {

    QString required;
    for (int i = 0; i < m_requiredArgs.size() ; i++) {
        if (m_requiredArgs.at(i).type() == QVariant::String )
            required.append(m_requiredArgs.at(i).toString() + " ");
        else if (m_requiredArgs.at(i).type() == QVariant::List) {
            required.append("{ ");
            for (int j = 0; j < m_requiredArgs.at(i).toList().size(); j++)
                required.append(m_requiredArgs.at(i).toList().at(j).toString() + " | ");
            required.append("} ");
        }
    }
    required.chop(1);
    return required;

}

QList<QVariant> Method::getRequiredArgsList() {
    return m_requiredArgs;
}

void Method::requires(QString arg) {
    m_requiredArgs.append(arg);
}

void Method::requiresOneOf(QStringList args) {
    QList<QVariant> list;
    list.append(args);
    m_requiredArgs.append(list);
}

bool Method::characters(const QString &str)
{
    m_currentText += str;
    return true;
}

bool Method::fatalError(const QXmlParseException &exception)
{
    qDebug("Parse error at line %d, column %d:\n%s", exception.lineNumber(),
                    exception.columnNumber(), exception.message().toStdString().c_str() );

    return false;
}

void Method::gotReply(QNetworkReply *reply) {

    // If you're using a single network access manager (in the factory),
    // you need to make sure you're handling the right request in case there
    // are multiple in flight
    if (m_reply == reply)
    {
        QXmlInputSource is;

        qDebug() << reply;

        if (reply->error() == QNetworkReply::NoError)
        {
            is.setData(reply->readAll());
            QXmlSimpleReader reader;
            reader.setContentHandler(this);
            reader.setErrorHandler(this);
            bool rc = reader.parse(is);

            if (rc)
                emit methodComplete((API::Method*)this);
            else
                emit methodFailed((API::Method*)this);
        }
        else
        {
            qDebug() << "API::Method::gotReply(); QNetworkReply: " << reply->errorString();
            qDebug() << reply->error();
            emit methodFailed((API::Method*)this);
        }
        reply->deleteLater();
    }
}



// These both need to be implemented in the derived classes to parse the XML
/*
bool Method::startElement(const QString &, const QString &,
                                  const QString &, const QXmlAttributes &) {
    return true;
}

bool Method::endElement(const QString &, const QString &,
                                const QString &) {
    return true;
}
*/


} // namespace API
