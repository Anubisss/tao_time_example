#ifndef __TIME_IMPL_H__
#define __TIME_IMPL_H__

#include <tao/ORB.h>
#include <tao/Basic_Types.h>

#include "TimeS.h"

/**
 *  @brief  This class is the implementation of the Time class which is
            defined in the Time.idl
            POA_Time is defined in TimeS.h file which is generated
            from the Time.idl by the IDL compiler.
 */
class TimeImpl : public POA_Time
{
    public:
        TimeImpl(CORBA::ORB_ptr orb) : _orb(orb) {}
        // TimeImpl(CORBA::ORB_ptr orb) : _orb(CORBA::ORB::_duplicate(orb)) {}

        /**
         *  @brief  Returns the current unix timestamp.
         *          This function is in Time.idl
         */
        /* virtual */ CORBA::ULongLong current_time();

        /**
         *  @brief  Shutdowns the server.
         *          This function is in Time.idl
         */
        /* virtual */ void shutdown();

    private:
        CORBA::ORB_ptr _orb;
};

#endif /* __TIME_IMPL_H__ */
