/* 
      PollTimer
      Copyright (C) 2016  Phillip J Schmidt
      
         This program is free software: you can redistribute it and/or modify
         it under the terms of the GNU General Public License as published by
         the Free Software Foundation, either version 3 of the License, or
         (at your option) any later version.
         
         This program is distributed in the hope that it will be useful,
         but WITHOUT ANY WARRANTY; without even the implied warranty of
         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
         GNU General Public License for more details.
         
         You should have received a copy of the GNU General Public License
         along with this program.  If not, see <http://www.gnu.org/licenses/>
         
 */


#include "PollTimer.h"


PollTimer::PollTimer( unsigned long Hz )
{
	period_us = 1000000UL / Hz;
	period_dt = 1.0f / float(Hz);
	
	nextExecute = micros() + period_us;
}


void PollTimer::start()
{
   nextExecute = micros() + period_us;
}


bool PollTimer::check()
{
   timeLast = timeNow;
   timeNow  = micros();

   if( timeNow < timeLast ) // timer roll-over detected -- must check every run
   {
      if( rollOverDetected )
      {
         rollOverDetected = false; // expected rollover, unset flag
      }
      else
      {
         while(nextExecute > period_us) // this may execute multiple times if the check is grossly late
         {
            nextExecute += period_us; // unexpected rollover -- nextExecute time was passed and timer rollover happened between checks
         }
         return true;
      }
   }   

   if( rollOverDetected )
   {
      // do nothing until timer rolls over as well
   }
	else if( timeNow > nextExecute )
	{
		nextExecute += period_us;
      
      if( nextExecute < period_us ) // future execute time roll-over detected
      {
         rollOverDetected = true; // set rollover flag
      }
      
		return true;
	}

	return false;
}


unsigned long PollTimer::us()
{
	return period_us;
}


float PollTimer::dt()
{
	return period_dt;
}


