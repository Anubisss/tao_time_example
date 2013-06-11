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
#include <ace/Time_Value.h>

#include <tao/ORB.h>
#include <tao/PortableServer/PortableServer.h>

#include <orbsvcs/orbsvcs/CosNamingC.h>

#include "TimeImpl.h"

int main(int argc, char* argv[])
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Server started.\n")));

    try
    {
        // inits the ORB
        CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv, "MyORB");
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB is created, id: %s.\n"), orb->id()));

        // obtains the RootPOA reference
        CORBA::Object_ptr objectPOA = orb->resolve_initial_references("RootPOA");
        // narrows down to get the correct reference
        PortableServer::POA_ptr POA = PortableServer::POA::_narrow(objectPOA);

        // this object's class is defined in the IDL file
        TimeImpl* timeFunc = new TimeImpl(orb);
        // converts the servant to a CORBA object ID
        PortableServer::ObjectId_var timeFuncID = POA->activate_object(timeFunc);

        // gets a real object/reference from the ID
        CORBA::Object_ptr timeFuncObject = POA->id_to_reference(timeFuncID);
        // narrows down...
        // this object should be registered with the Naming Service
        CORBA::Object_ptr tFunc = CORBA::Object::_narrow(timeFuncObject);

        // obtains the Naming Service
        CORBA::Object_ptr nameService = orb->resolve_initial_references("NameService");
        // nameing context
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(nameService);	

        // fills the Name structure
        // this will be registered with the Naming Service
        CosNaming::Name bindName(1);
        bindName.length(1);
        // name of the object, so in client side this name should be used to resolve the Time object
        bindName[0].id = CORBA::string_dup("TestTime");

        // registers/binds the object with the Naming Service
        namingContext->rebind(bindName, tFunc);

        // actives the POAManager to process incoming requests
        PortableServer::POAManager_ptr POAMgr = POA->the_POAManager();
        POAMgr->activate();

        // runs the ORB
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB is running.\n")));
        orb->run();

        POA->destroy(true, true);
        orb->destroy();
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB destroyed.\n")));

    }
    catch (CORBA::ORB::InvalidName const& ex)
    {
        ex._tao_print_exception("Exception: InvalidName\n\n");
        system("pause");
        return 1;
    }
    catch (...)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) exception caught!\n\n")));
        system("pause");
        return 1;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Server exitted.\n\n")));
    system("pause");
    return 0;
}
