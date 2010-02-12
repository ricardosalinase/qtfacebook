#include "updatepoller.h"

UpdatePoller::UpdatePoller(UserInfo *info, QObject *parent) :
        Worker(parent),
        m_userInfo(info)
{
}


void UpdatePoller::init() {

    m_factory = new API::Factory(m_userInfo, this);


}
