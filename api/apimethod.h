#ifndef APIMETHOD_H
#define APIMETHOD_H

/*******************************************************************
* This is the base class for all API Methods. All actual methods   *
* subclass it.                                                     *
*******************************************************************/

#include <QString>
#include <QMap>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QList>
#include <QStringList>
#include <QXmlDefaultHandler>
#include <QNetworkReply>

#include <userinfo.h>


namespace API
{

    /*! @class API::Method
    * This is the base class that encapsulate Facebook API calls. It can not be instantiated directly. It inherits
    * from QXmlDefaultHandler to provide XML parsing for the data returned by Facebook.
    *
    * When creating a new API call, you must implement getMethodName() as well as two classes from QXmlDefaultHandler:
    * startElement() and endElement(). You will also want to set the required parameters in the contructor of your derived
    * class.
    */
    class Method : public QObject, public QXmlDefaultHandler
    {
        Q_OBJECT

    public:
        explicit Method(QObject *parent = 0);
        virtual ~Method();

        /*!
         * Sets the QNetworkAccessManager to be used for this API call
         */
        void setAccessManager(QNetworkAccessManager *manager);
        /*!
         * Sets the UserInfo object to be used for this API call
         */
        void setUserInfo(UserInfo *userInfo);
        /*!
         * Returns a list (as a QString) of parameters required for this API call
         */
        QString getRequiredArgsString();
        /*!
         * Returns a list of parameters required for this API call, encapsulated in QVariant Objects
         */
        QList<QVariant> getRequiredArgsList();
        /*!
         * Causes the post operation to be sent to the Facebook REST server. If for some reason the post
         * can not be sent (missing parameters, for example), the result will be false and the error set in
         * m_errStr (see getErrStr() below)
         */
        bool execute();
        /*!
         * Returns a QString containing information if there was an error sending the request.
         */
        QString getErrorStr();
        /*!
         * Used to set the arguments for the POST to the Facebook REST server as specified in the Facebook API
         */
        void setArgument(QString arg, QString value);
        /*!
         * @overload
         */
        void setArgument(QString arg, int value);
        /*!
         * @overload
         */
        void setArgument(QString arg, QStringList value);
        /*!
         * Overridden method from QXmlDefaultHandler. The XML parser uses this to place parsed data into m_currentText
         */
        bool characters(const QString &str);
        /*!
         * Overridden method from QXmlDefaultHandler. Parsing failures are handled here.
         */
        bool fatalError(const QXmlParseException &exception);
        /*!
         * Overridden method from QXmlDefaultHandler. Called when the parser finds a new element (opening tag).
         * This must be implemented in derived classes to process the XML data returned by Facebook.
         */
        virtual bool startElement(const QString &namespaceURI, const QString &localName,
                          const QString &qName, const QXmlAttributes &attributes) = 0;
        /*!
         * Overridden method from QXmlDefaultHandler. Called when the parser finds the end of an element (closing tag)
         * This must be implemented in derived classes to process the XML data returned by Facebook.
         */
        virtual bool endElement(const QString &namespaceURI, const QString &localName,
                        const QString &qName) = 0;

        /*!
         * Returns the name of this API call. It is recommended this match the actual Facebook method
         * ( "notifications.getList" as an example ). This is required in derived classes.
         */
        virtual QString getMethodName() = 0;


    public slots:
        /*!
         * Used by the internal QNetworkAccessManager
         */
        void gotReply(QNetworkReply *reply);

    signals:
        /*!
         * Sent once XML parsing is complete. Note that this sends the method cast to the base class (API::Method)
         * and needs to be cast back to the appropriate derived class upon receoption. If using the API::Factory
         * you shouldn't connect to this signal, but rather the ones provided there for each derived class.
         */
        void methodComplete(API::Method *thisMethod);


    private:
        /*!
         * Used internally to store error information. Returned by getErrStr()
         */
        QString m_errStr;
        /*!
         * Called during execute() this method varidates that all required parameters have been set and
         * that internal objects such as UserInfo are set.
         */
        bool validate();



    protected:
        /*!
         * Method used in FQL queries to prepare the FQL statement. Is it called in exec() prior to making the
         * POST to the Facebook API. The default implementation does nothing.
         */
        virtual bool prepare();
        /*!
         * The UserInfo object
         */
        UserInfo *m_userInfo;
        /*!
         * The list that contains the parameters and values that will be passed to the API call
         */
        QMap<QString, QVariant> m_argMap;
        /*!
         * the list of required paramaters for this API call. See requires() and requiresOneOf()
         */
        QList<QVariant> m_requiredArgs;
        /*!
         * The QNetworkAccessManager for this call
         */
        QNetworkAccessManager *m_manager;
        /*!
         * Used by the XML parser to store the data for the tag currently being parsed
         */
        QString m_currentText;

        /*!
         * Used in the constructor of derived classes to set the required parameters for the API call
         */
        void requires(QString arg);
        /*!
         * Used in the constructor of derived classes to set the required parameters for the API call.
         * This version is used for when one out of a set of N parameters is required for the call.
         */
        void requiresOneOf(QStringList args);








    };
};
#endif // APIMETHOD_H
