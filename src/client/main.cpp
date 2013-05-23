#include <ace/Log_Msg.h>

#include <tao/ORB.h>

#include <orbsvcs/orbsvcs/CosNamingC.h>

#include "TimeC.h"

ACE_UINT32 GetMSTime() { return GetTickCount(); }
ACE_UINT32 GetMSTimeDiff(ACE_UINT32 oldMSTime, ACE_UINT32 newMSTime)
{
    // getMSTime() has limited data range and this is case when it overflows in this tick
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

int main(int argc, char* argv[])
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Client started.\n")));

    try
    {
        // inits the ORB
        CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv, "MyORB");
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB is created, id: %s.\n"), orb->id()));

        // obtains the Naming Service reference
        CORBA::Object_ptr namingService = orb->resolve_initial_references("NameService");
        // narrows down the reference to obtains a reference to the naming context
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(namingService);

        // TestTime is the name which registered by the server
        CosNaming::Name bindName(1);
        bindName.length(1);
        bindName[0].id = CORBA::string_dup("TestTime");

        // basically here resolves the TestTime from the Naming Service
        // and gets a Time object which is defined in the Time.idl
        Time_ptr timeClient = Time::_narrow(namingContext->resolve(bindName));

        CORBA::ULongLong serverTime = 0;
        // simple latency calculation
        ACE_UINT32 latency1 = GetMSTime();

        // gets the time from the server
        serverTime = timeClient->current_time();

        ACE_UINT32 latency2 = GetMSTime();

        // %Q is uint64
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Server time: %Q Latency: %u ms\n"), serverTime, GetMSTimeDiff(latency1, latency2)));

        // timeClient->shutdown();
        orb->shutdown();
        orb->destroy();

    }
    catch (...)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) exception caught!\n\n")));
        system("pause");
        return 1;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Client exitted.\n\n")));
    system("pause");
    return 0;
}
