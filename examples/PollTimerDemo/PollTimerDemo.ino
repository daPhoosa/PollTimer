#include <PollTimer.h>

PollTimer criticalTask( 1000 );
PollTimer moderateTask(  100 );
PollTimer maintenanceTask( 1 );

unsigned long dummyTestVariable;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(250000);
  while( !Serial ); // wait for serial port to finish initializing

  criticalTask.start();
  moderateTask.start();
  maintenanceTask.start();  
}

void loop() {
  // put your main code here, to run repeatedly:

  //  The use an if-else priority scheme insure the highest priority operations
  //    get the first chance at running again after any other operation executes
  if( criticalTask.check() )
  {
    dummyTestVariable++;

    criticalTask.collectStats();
  }
  else if( moderateTask.check() )
  {
    dummyTestVariable = dummyTestVariable % 3;

    moderateTask.collectStats();
  }
  else if( maintenanceTask.check() )
  {
    Serial.print(" Test Variable: "); Serial.println( dummyTestVariable );
    
    criticalTask.displayStats();

    moderateTask.displayStats();

    Serial.println("");
  }

}
