/*
 * This file is part of tao_time_example.
 *
 * tao_time_example is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * tao_time_example is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with tao_time_example.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ace/Log_Msg.h>

#include <tao/ORB.h>

#include <orbsvcs/orbsvcs/CosNamingC.h>

#include "TimeC.h"

// this function should used with GetMSTimeDiff
ACE_UINT32 GetMSTime() { return GetTickCount(); }
// returns the diff of two MS time
ACE_UINT32 GetMSTimeDiff(ACE_UINT32 oldMSTime, ACE_UINT32 newMSTime)
{
    // GetMSTime() has limited data range and this is case when it overflows in this tick
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

        // fills the Name structure
        // this will be resolved by the Naming Service
        // in server side this is registered/binded by the Naming Service
        CosNaming::Name bindName(1);
        bindName.length(1);
        // TestTime is the name which registered by the server
        bindName[0].id = CORBA::string_dup("TestTime");

        // basically here resolves the TestTime from the Naming Service
        // and gets a Time object which is defined in the Time.idl
        Time_ptr timeClient = Time::_narrow(namingContext->resolve(bindName));

        CORBA::ULongLong serverTime = 0;
        // simple latency calculation
        ACE_UINT32 latency1 = GetMSTime();

        // gets the time from the server
        serverTime = timeClient->current_time();

        // latency calculation part 2
        ACE_UINT32 latency2 = GetMSTime();

        // %Q is uint64
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Server time: %Q Latency: %u ms\n"), serverTime, GetMSTimeDiff(latency1, latency2)));

        // timeClient->shutdown(); calling this then the server shuts down
        orb->shutdown();
        orb->destroy();

    }
    // let's catch everyting
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
