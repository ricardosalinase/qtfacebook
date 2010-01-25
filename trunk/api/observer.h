#ifndef OBSERVER_H
#define OBSERVER_H

#include <QNetworkReply>
#include <QWidget>

namespace API {


class ObserverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObserverWidget(QWidget *parent = 0) : QWidget(parent) {};

public slots:
    virtual void processMethodResults(QNetworkReply *reply) = 0;


};

class ObserverObject : public QObject
{
    Q_OBJECT

public:
    explicit ObserverObject(QObject *parent = 0) : QObject(parent) {};

public slots:
    virtual void processMethodResults(QNetworkReply *reply) = 0;

};


} // namespace API

#endif // OBSERVER_H
