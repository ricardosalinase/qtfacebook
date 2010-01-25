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

#include <userinfo.h>

namespace API
{

    class Method : public QObject
    {
        Q_OBJECT

    public:
        explicit Method(QObject *parent = 0);
        virtual ~Method();

        void setAccessManager(QNetworkAccessManager *manager);
        void setUserInfo(UserInfo *userInfo);
        QString getRequiredArgsString();
        QList<QVariant> getRequiredArgsList();
        QNetworkReply * execute();
        QString getErrorStr();

        void setArgument(QString arg, QString value);
        void setArgument(QString arg, int value);


    private:
        QString m_errStr;
        bool validate();


    protected:
        UserInfo *m_userInfo;
        QMap<QString, QVariant> m_argMap;
        QList<QVariant> m_requiredArgs;
        QNetworkAccessManager *m_manager;
        QString m_methodName;


        void requires(QString arg);
        void requiresOneOf(QStringList args);









    };
};
#endif // APIMETHOD_H
