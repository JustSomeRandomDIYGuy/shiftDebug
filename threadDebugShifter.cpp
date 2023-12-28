#include "threadDebugShifter.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ---------------------------------------------------------------------------
//                              Constructor
// ---------------------------------------------------------------------------
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


ThreadDebugShifter::ThreadDebugShifter(uint8_t latchPin ){
    _latchPin = latchPin;	
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ---------------------------------------------------------------------------
//                            Class Functions
// ---------------------------------------------------------------------------
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ---------------------------------------------------------------------------
//                           begin
// ---------------------------------------------------------------------------
/*
 *  Transmits thread start signal. So transmitting threadID via shift out and than
 *  keep data line on high until sendThreadStopSignal() is called.
 */
void	ThreadDebugShifter::begin(){		
	pinMode(_latchPin, OUTPUT);
	digitalWrite(_latchPin, HIGH);
	
	#if USE_DIRECT_PORT_MANIPULATION
		_latchOutRegister = portOutputRegister(digitalPinToPort(_latchPin));
		_latchPinMask = digitalPinToBitMask(_latchPin);
	#endif	
	
	#ifndef TDS_USE_SHIFT_OUT 
		SPI.begin();	
	#endif
	_write16bits();
		
}

// ---------------------------------------------------------------------------
//                           write16bits
// ---------------------------------------------------------------------------
/*
 *  
 */
void ThreadDebugShifter::_write16bits(){    
	if (_debugEnabled){		
		#if USE_DIRECT_PORT_MANIPULATION
			*_latchOutRegister &= ~_latchPinMask;
		#else
			digitalWrite(_latchPin, LOW);
		#endif
		
		#if USE_BEGIN_AND_END_TRANSMISSION
			#ifndef TDS_USE_SHIFT_OUT 
				SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));			
			#endif
		#endif
		
		// transmit 2 bytes for 16 outputs
		#ifndef TDS_USE_SHIFT_OUT 
			SPI.transfer(_pinStates >> 8);
			SPI.transfer(_pinStates);
		#else
			shiftOut(12, 13, MSBFIRST, _pinStates >> 8);
			shiftOut(12, 13, MSBFIRST, _pinStates);
		#endif
		

		#if USE_BEGIN_AND_END_TRANSMISSION
			#ifndef TDS_USE_SHIFT_OUT 
				SPI1.endTransaction();			
			#endif
		#endif
				
		// set latch pin to HIGH
		#if USE_DIRECT_PORT_MANIPULATION
			*_latchOutRegister |= _latchPinMask;
		#else
			digitalWrite(_latchPin, HIGH);
		#endif
	}
} 

// ---------------------------------------------------------------------------
//                           sendThreadStartSignal
// ---------------------------------------------------------------------------
/*
 *  Transmits thread start signal. So transmitting threadID via shift out and than
 *  keep data line on high until sendThreadStopSignal() is called.
 */
void ThreadDebugShifter::sendThreadStartSignal(uint8_t threadID){
	if (_debugEnabled){
		if ( ( threadID >= _firstAvailableChannel ) &&  ( threadID <= 15 ) ){
			uint16_t bitMask = 1 << threadID;
			_pinStates = _pinStates | bitMask;
			_checkAllPins();			
			_write16bits();
		}
	}
}

// ---------------------------------------------------------------------------
//                           sendThreadStartSignal
// ---------------------------------------------------------------------------
/*
 *  Transmits thread start signal. So transmitting threadID via shift out and than
 *  keep data line on high until sendThreadStopSignal() is called.
 */
void ThreadDebugShifter::sendThreadStopSignal(uint8_t threadID){
	if (_debugEnabled){
		if ( ( threadID >= _firstAvailableChannel ) &&  ( threadID <= 15 ) ){
			uint16_t bitMask = ~(1 << threadID);
			_pinStates = _pinStates & bitMask;
			_checkAllPins();
			_write16bits();
		}
	}
}

// ---------------------------------------------------------------------------
//                           checkAllPins
// ---------------------------------------------------------------------------
/*
 *  
 */
void ThreadDebugShifter::_checkAllPins(){    
	if (_debugEnabled){
		if (_sumThreadActOnCh0) {
			if ( _pinStates >> 1 ){		
				_pinStates = _pinStates | 1;
			}
			else{
				_pinStates = _pinStates & (~1);
			}
		}
	}
}

// ---------------------------------------------------------------------------
//                           disableDebugging
// ---------------------------------------------------------------------------
/*
 *  
 */
void ThreadDebugShifter::disableDebugging(){    
	_debugEnabled = false;
}


// ---------------------------------------------------------------------------
//                           enableSumThreadActivityOnCh0
// ---------------------------------------------------------------------------
/*
 *  
 */
void ThreadDebugShifter::enableSumThreadActivityOnCh0(){    
	_sumThreadActOnCh0 = true;
	_firstAvailableChannel = 1;
}

// ---------------------------------------------------------------------------
//                           disableSumThreadActivityOnCh0
// ---------------------------------------------------------------------------
/*
 *  
 */
void ThreadDebugShifter::disableSumThreadActivityOnCh0(){    
	_sumThreadActOnCh0 = false;
	_firstAvailableChannel = 0;
}

				

// ---------------------------------------------------------------------------
//                           getStates
// ---------------------------------------------------------------------------
/*
 *  
 */
uint16_t ThreadDebugShifter::getStates(){    
	return _pinStates; 
}