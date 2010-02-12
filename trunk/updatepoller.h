#ifndef UPDATEPOLLER_H
#define UPDATEPOLLER_H

#include "userinfo.h"
#include "util/worker.h"
#include "api/factory.h"

class UpdatePoller : public UTIL::Worker
{
public:
    UpdatePoller(UserInfo *info, QObject *parent = 0);
    void init();

private:
    UserInfo *m_userInfo;
    API::Factory *m_factory;

};

#endif // UPDATEPOLLER_H
