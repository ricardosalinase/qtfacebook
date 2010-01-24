#ifndef FACTORY_H
#define FACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

// Simple factory created as a singleton so that there is a single
// instance of QNetworkAcceessManager



namespace API {

class factory : public QObject
{
Q_OBJECT
public:
    explicit factory(QObject *parent = 0);


signals:

public slots:

private:
    QNetworkAccessManager *m_manager;

};

} // namespace API

#endif // FACTORY_H
