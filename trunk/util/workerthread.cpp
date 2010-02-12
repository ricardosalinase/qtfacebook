#include "workerthread.h"

namespace UTIL {

WorkerThread::WorkerThread(Worker *worker, QObject *parent) :
    QThread(parent),
    m_worker(worker)
{

    m_worker->moveToThread(this);
}

void WorkerThread::run() {

    m_worker->init();

    exec();

}

} // namespace UTIL
