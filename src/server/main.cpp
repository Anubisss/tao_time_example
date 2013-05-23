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

        TimeImpl* TimeFunc = new TimeImpl(orb);
        // PortableServer::ServantBase_var owner_transfer(TimeFunc);
        PortableServer::ObjectId_var TimeFuncID = POA->activate_object(TimeFunc);

        // CORBA::Object_ptr TimeFuncObject = POA->id_to_reference(TimeFuncID.in());
        CORBA::Object_ptr TimeFuncObject = POA->id_to_reference(TimeFuncID);
        CORBA::Object_ptr TFunc = CORBA::Object::_narrow(TimeFuncObject);

        /*
        // IOR
        CORBA::String_var IOR = Orb->object_to_string(TFunc);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) IOR created.\n")));
        FILE *IOR_Out = ACE_OS::fopen("server.ior", "w");
        ACE_OS::fprintf(IOR_Out, IOR.in());
        ACE_OS::fclose(IOR_Out);
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) IOR writed to file.\n")));
        */


        // Register with Naming Service
        CORBA::Object_ptr NameService = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var NamingContext = CosNaming::NamingContext::_narrow(NameService);	
	
        CosNaming::Name BindName(1);
        BindName.length(1);
        BindName[0].id = CORBA::string_dup("TestTime");

        NamingContext->rebind(BindName, TFunc);



        // active the POAManager to process incoming requests
        PortableServer::POAManager_ptr POAMgr = POA->the_POAManager();
        POAMgr->activate();
        // run the ORB
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB is running.\n")));
        orb->run();

        POA->destroy(true, true);
        // be kind and serve the requests
        // Orb->shutdown(true);
        // ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB shutdowned.\n")));
        orb->destroy();
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) ORB destroyed.\n")));

    }
    catch (CORBA::ORB::InvalidName const& Exception)
    {
        Exception._tao_print_exception("Exception: InvalidName\n\n");
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
