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



    class Method : public QObject, public QXmlDefaultHandler
    {
        Q_OBJECT

    public:
        explicit Method(QObject *parent = 0);
        virtual ~Method();

        void setAccessManager(QNetworkAccessManager *manager);
        void setUserInfo(UserInfo *userInfo);
        QString getRequiredArgsString();
        QList<QVariant> getRequiredArgsList();
        bool execute();
        QString getErrorStr();

        void setArgument(QString arg, QString value);
        void setArgument(QString arg, int value);

        bool characters(const QString &str);
        bool fatalError(const QXmlParseException &exception);

        // Override methods from QXmlDefaultHandler
        virtual bool startElement(const QString &namespaceURI, const QString &localName,
                          const QString &qName, const QXmlAttributes &attributes) = 0;
        virtual bool endElement(const QString &namespaceURI, const QString &localName,
                        const QString &qName) = 0;

        virtual QString getMethodName() = 0;

    public slots:
        void gotReply(QNetworkReply *reply);

    signals:
        void methodComplete(API::Method *thisMethod);


    private:
        QString m_errStr;
        bool validate();



    protected:
        UserInfo *m_userInfo;
        QMap<QString, QVariant> m_argMap;
        QList<QVariant> m_requiredArgs;
        QNetworkAccessManager *m_manager;
        QString m_currentText;

        void requires(QString arg);
        void requiresOneOf(QStringList args);









    };
};
#endif // APIMETHOD_H
