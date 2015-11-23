/* 
 * PollTimer
 * by Phillip Schmidt
 * v1.0
 *	
 */


#include "PollTimer.h"


PollTimer::PollTimer(unsigned long Hz)
{
	period_us = 1000000UL / Hz;
	period_dt = 1.0f / float(Hz);
	
	nextExecute = micros() + period_us;
}


void PollTimer::start()
{
	unsigned long timeNow = micros();
	
	nextExecute = timeNow + period_us;
	
	if(nextExecute < timeNow)  // detect roll over of future execute time
	{
		NoRolloverFlag = 0; // unset flag to prevent execution until timer also rolls over
	}
	else
	{
		NoRolloverFlag = 1; // flag set for normal operation
	}
	
	lastCheckTime = timeNow;
}


bool PollTimer::check()
{
	unsigned long timeNow = micros();
	
	if(timeNow < lastCheckTime) // reset flag once timer rolls over
	{
		NoRolloverFlag = 1;
	}
	
	if(timeNow > nextExecute && NoRolloverFlag)
	{
		nextExecute += period_us;
		
		if(nextExecute < timeNow)  // detect roll over of future execute time
		{
			NoRolloverFlag = 0; // unset flag to prevent execution until timer also rolls over
		}
		
		lastCheckTime = timeNow;
		
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


