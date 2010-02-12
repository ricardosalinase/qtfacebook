#ifndef WORKER_H
#define WORKER_H

#include <QObject>

namespace UTIL {

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    virtual void init() = 0;



};


} // end namespace UTIL



#endif // WORKER_H
