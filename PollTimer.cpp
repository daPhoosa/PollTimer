/* 
 * PollTimer
 * by Phillip Schmidt
 * v1.0
 *	
 *	Object Creation: PollTimer TimerObject(100UL); // run this loop at 100Hz
 *
 *	Time Check: TimerObject.check()  // this will return true only once per cycle
 *
 *	Loop time integer us: TimerObject.us();
 *
 *	Loop time float decimal seconds: TimerObject.dt();
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
   
   if( rollOverDetected )
   {
      if( timeNow < timeLast ) // timer roll-over detected
      {
         rollOverDetected = false;
      }      
   }
	else if( timeNow > nextExecute )
	{
		nextExecute += period_us;
      
      if( nextExecute < period_us ) // future execute time roll-over detected
      {
         rollOverDetected = true;
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


