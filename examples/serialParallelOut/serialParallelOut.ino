/*
 *  Name:     serialParallelOut.ino
 *  Created:	28.12.2023 
 *  Author:    DLR\stra_ma
 * */

// --- Compile-Time Switches ---

// --- Libraries ---
#include <arduino.h>
#include <threadDebugShifter.h>

// --- CONSTANTS ---
const uint8_t   THREAD_DEBUG_LATCH_PIN = 11;
const uint8_t   BITS = 15;  // number of effected bits
const uint32_t  TIME1 = 30;
const uint32_t  TIME2 = 100;

// --- VARIABLES ---

// --- STRUCTS ---

// --- OBJECTES ---
ThreadDebugShifter tD(THREAD_DEBUG_LATCH_PIN);

// --- FUNCTION PROTOTYPES ---
void printPinStates2Serials();

// ------------------------------------------------------------------------
//                                S E T U P
// ------------------------------------------------------------------------
void setup(){
  tD.begin();
  // tD.enableSumThreadActivityOnCh0();
  tD.disableSumThreadActivityOnCh0();
  SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0)); // max realistic SPI Clock of 12MHz for Adafruit Feather M0 Express (µC-clock at 48MHz)
	Serial.begin(115200);
	Serial1.begin(115200);
}

// ------------------------------------------------------------------------
//                                L O O P
// ------------------------------------------------------------------------

void loop(){
  static uint32_t counter = 0;
	counter++;
  
  // initial trigger for logic analyser on pin 12 
  tD.sendThreadStartSignal(15);
  tD.sendThreadStopSignal(15);  
  // generation of two fast on-off cycles in order to measure the time that one bit-setting will require
  tD.sendThreadStartSignal(0);
  tD.sendThreadStopSignal(0);  
  tD.sendThreadStartSignal(0);
  tD.sendThreadStopSignal(0);

  for (int j = 0; j < 2 ; j++){
    for (int i = 0; i < BITS ; i++)
    {    
        tD.sendThreadStartSignal(i);
        delay(TIME1);
        printPinStates2Serials();
        tD.sendThreadStopSignal(i);
    }
    for (int i = BITS - 1; i >= 0 ; i--)
    {    
        tD.sendThreadStartSignal(i);
        delay(TIME1);
        printPinStates2Serials();
        tD.sendThreadStopSignal(i);
    }
  }
  
  delay(TIME2);
  
  for (int j = 0; j < 2 ; j++){
    for (int i = 0; i < BITS ; i++)
    {    
        tD.sendThreadStartSignal(i);
        printPinStates2Serials();
        delay(TIME1);
    }
    
    delay(TIME2);
    
    for (int i = 0; i < BITS ; i++)
    {    
        tD.sendThreadStopSignal(i);
        printPinStates2Serials();
        delay(TIME1);
    }
    
    delay(TIME2);
    
    for (int i = BITS - 1 ; i >= 0 ; i--)
    {    
        tD.sendThreadStartSignal(i);
        printPinStates2Serials();
        delay(TIME1);
    }
    
    delay(TIME2);
    
    for (int i = BITS - 1 ; i >= 0 ; i--)
    {    
        tD.sendThreadStopSignal(i);
        printPinStates2Serials();
        delay(TIME1);
    }
  }
	
	delay(2*TIME2);
  
}

// ------------------------------------------------------------------------
//                      SUB - ROUTINES
// ------------------------------------------------------------------------


// ........................................................................
void printPinStates2Serials(){
  Serial1.write( tD.getStates() >> 8 );
  Serial1.write( tD.getStates()  );
  Serial.println( tD.getStates() , BIN );        
}
