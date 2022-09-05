
/*
    Name:     debugShifter.ino
    Created:	11.03.2020 10:58:29
    Author:     DLR\stra_ma
*/

#include <arduino.h>
#include <threadDebugShifter.h>

#define THREAD_1	6
#define THREAD_2	1
#define THREAD_3	2
#define THREAD_4	4
#define THREAD_5	5

#define THREAD_DEBUG_LATCH_PIN  A4 

#define TIME1 30
#define TIME2 100

ThreadDebugShifter tD(THREAD_DEBUG_LATCH_PIN);

void setup()
{
	tD.begin();
  tD.disableSumThreadActivityOnCh0();
	Serial.begin(115200);
	//while(!Serial);		
}

void loop()
{
	static uint32_t counter = 0;
	counter++;

  Serial.println(counter);
  for (int j = 0; j < 5 ; j++){
    for (int i = 0; i < 8 ; i++)
    {    
        tD.sendThreadStartSignal(i);
        delay(TIME1);
        tD.sendThreadStopSignal(i);
    }
    for (int i = 7; i >= 0 ; i--)
    {    
        tD.sendThreadStartSignal(i);
        delay(TIME1);
        tD.sendThreadStopSignal(i);
    }
  }
  
  delay(TIME2);
  
  for (int j = 0; j < 5 ; j++){
    for (int i = 0; i < 8 ; i++)
    {    
        tD.sendThreadStartSignal(i);
        delay(TIME1);
    }
    
    delay(TIME2);
    
    for (int i = 0; i < 8 ; i++)
    {    
        tD.sendThreadStopSignal(i);
        delay(TIME1);
    }
    
    delay(TIME2);
    
    for (int i = 7; i >= 0 ; i--)
    {    
        tD.sendThreadStartSignal(i);
        delay(TIME1);
    }
    
    delay(TIME2);
    
    for (int i = 7; i >= 0 ; i--)
    {    
        tD.sendThreadStopSignal(i);
        delay(TIME1);
    }
  }
	
	delay(2*TIME2);
}
