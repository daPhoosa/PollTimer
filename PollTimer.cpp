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
   uint32_t timeNow = micros();
   if(nextExecute - timeNow > period_us)  // this logic handles timer roll-over properly
   {
      nextExecute += period_us;
      statsTimeStart = timeNow;
      return true;
   }
   
   return false;
}


bool PollTimer::precheck(uint32_t earlyTime) // this will return true if the time until next execute is less than 'earlyTime', does not reset nextExecute
{
   if(nextExecute - micros() > period_us - earlyTime)  
   {
      return true;
   }
   
   return false;      
}


void PollTimer::collectStats()
{
   uint32_t runTime = micros() - statsTimeStart;

   cycleCount++;
   
   if( runTime > maxTime ) maxTime = runTime;
   if( runTime < minTime ) minTime = runTime;
   avgCollector += runTime;  // to do: add rollover detection, maybe
}


uint32_t PollTimer::getMaxTime()
{
   return maxTime;
}


uint32_t PollTimer::getMinTime()
{
   return minTime;
}


uint32_t PollTimer::getAvgTime()
{
   return avgCollector / cycleCount;
}


uint32_t PollTimer::getCount()
{
   return cycleCount;
}


void PollTimer::displayStats()
{
   Serial.print("MIN: ");Serial.println(getMinTime());
   Serial.print("AVG: ");Serial.println(getAvgTime());
   Serial.print("MAX: ");Serial.println(getMaxTime());
   Serial.print("CNT: ");Serial.println(getCount());
   Serial.println("");
}


unsigned long PollTimer::us()
{
	return period_us;
}


float PollTimer::dt()
{
	return period_dt;
}


