#include <ace/Log_Msg.h>

#include <ctime>

#include "TimeImpl.h"

/* virtual */ CORBA::ULongLong TimeImpl::current_time()
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Received current_time command.\n")));
    CORBA::ULongLong now = time(NULL);
    return now;
}

/* virtual */ void TimeImpl::shutdown()
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Received shutdown command.\n")));
    _orb->shutdown(false);
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB is shutdowned.\n")));
}
