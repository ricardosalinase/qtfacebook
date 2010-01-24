#ifndef APIMETHOD_H
#define APIMETHOD_H

/*******************************************************************
* This is the base class for all API Methods. All actual methods
* subclass it.
*******************************************************************/

#include <QString>
#include <QMap>
#include <QVariant>
#include <QNetworkAccessManager>

#include <userinfo.h>
#include "observer.h"

namespace API
{

    class Method : public QObject
    {
        Q_OBJECT

    public:
        Method(UserInfo *userInfo, QNetworkAccessManager *m_manager, QObject *parent = 0);
        virtual ~Method();

        bool execute(Observer *observer);

    protected:
        UserInfo *m_userInfo;
        QMap<QString, QVariant> m_argMap;
        QNetworkAccessManager *m_manager;
        QString m_methodName;
        virtual bool validateArgs();
        virtual QString getMethodName() = 0;



    };
};
#endif // APIMETHOD_H
