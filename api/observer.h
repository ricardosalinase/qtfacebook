#ifndef OBSERVER_H
#define OBSERVER_H

#include <QNetworkReply>

namespace API {

    class Observer : public QObject
{
    Q_OBJECT

public:

public slots:
    virtual void processMethodResults(QNetworkReply *reply) = 0;


};

} // namespace API

#endif // OBSERVER_H
