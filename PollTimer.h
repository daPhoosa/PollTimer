/* 
 * PollTimer
 * by Phillip Schmidt
 * v1.0
 *
 */


#ifndef PollTimer_h
#define PollTimer_h

#include <arduino.h>

class PollTimer
{
	public:
	
		PollTimer(unsigned long Hz);
		bool check();
		void start();
		
		unsigned long us();	// return microseconds
		float dt();				// return decimal seconds
		
		
	private:
		unsigned long period_us;	// microseconds
		float period_dt;			// decimal seconds
		
		unsigned long lastCheckTime;
		uint8_t NoRolloverFlag;
		
		unsigned long nextExecute;

};

#endif