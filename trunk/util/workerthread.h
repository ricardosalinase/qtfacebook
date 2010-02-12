#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "util/worker.h"

namespace UTIL {

class WorkerThread : public QThread
{
Q_OBJECT
public:
    explicit WorkerThread(Worker *, QObject *parent = 0);
    void run();
signals:

public slots:

private:
    Worker *m_worker;

};

} // namespace UTIL

#endif // WORKERTHREAD_H
