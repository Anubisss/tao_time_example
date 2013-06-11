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
