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

   resetStats();
}


void PollTimer::start()
{
   uint32_t timeNow = micros();
   nextExecute = timeNow + period_us;
}


void PollTimer::start(uint32_t delay)
{
   start();
   nextExecute += delay;
}


bool PollTimer::check()
{
   uint32_t timeNow = micros();
   uint32_t timeCheck = nextExecute - timeNow;
   if( timeCheck > period_us)  // this logic handles timer roll-over properly
   {
      if( statsCount ) // throw away first sample ( or bypass if not collecting stats )
      {
         uint32_t lateTime = 4294967296UL - timeCheck;
         if( lateTime > maxLateTime ) maxLateTime = lateTime;
         lateTimeCollector += lateTime;
         runCount++;
      }
      
      nextExecute += period_us;
      statsTimeStart = timeNow;
      return true;
   }
   
   return false;
}


bool PollTimer::precheck(uint32_t earlyTime) // this will return true if the time until next execute is less than 'earlyTime', does not reset nextExecute
{
   uint32_t check = nextExecute - micros();
   if( check < earlyTime || check > period_us )  // (also returns true if already past next check)
   {
      return true;
   }
   
   return false;      
}


void PollTimer::collectStats()
{
   uint32_t runTime = micros() - statsTimeStart;

   
   if( statsCount ) // throw away first sample
   {
      if( runTime > maxTime ) maxTime = runTime;
      if( runTime < minTime ) minTime = runTime;
      avgCollector += runTime;  // to do: add rollover detection, maybe
   }
 
   statsCount++;
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
   if( statsCount > 1 )
   {
      averageRun = avgCollector / (statsCount - 1);
      return averageRun;
   }
   return 0;
}


uint32_t PollTimer::getAvgLate()
{
   if( runCount > 1 )
   {
      return lateTimeCollector / (runCount - 1);
   }
   return 0;
}


uint32_t PollTimer::getMaxLateTime()
{
   return maxLateTime;
}


uint32_t PollTimer::getCount()
{
   return statsCount;
}


float PollTimer::getPctCPU()
{
   return float( averageRun * 100 ) / float( period_us );
}


void PollTimer::displayStats()
{
   Serial.print("RUN MIN:  ");Serial.println(getMinTime());
   Serial.print("RUN AVG:  ");Serial.println(getAvgTime());
   Serial.print("RUN MAX:  ");Serial.println(getMaxTime());
   Serial.print("RUN CNT:  ");Serial.println(getCount());
   Serial.print("LATE AVG: ");Serial.println(getAvgLate());
   Serial.print("LATE MAX: ");Serial.println(getMaxLateTime());
   Serial.print("CPU PCT : ");Serial.println(getPctCPU(), 1);
   Serial.println("");
}


void PollTimer::resetStats()
{
   maxTime = 0;
   minTime = 10000000; // 1Hz is realistic worst case, so 10s should be ok... right?
   avgCollector = 0;
   lateTimeCollector = 0;
   runCount = 0;
   statsCount = 0;
   maxLateTime = 0;
}

unsigned long PollTimer::us()
{
	return period_us;
}


float PollTimer::dt()
{
	return period_dt;
}


