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

#ifndef __TIME_IMPL_H__
#define __TIME_IMPL_H__

#include <tao/ORB.h>
#include <tao/Basic_Types.h>

#include "idl/TimeS.h"

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
